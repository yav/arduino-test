#include <SPI.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <avr/pgmspace.h>

#include "Color.h"
#include "YTime.h"

#define STMPE_CS 8
#define TFT_DC   9
#define TFT_CS  10

// Used for software SPI
#define LIS3DH_CLK 5
#define LIS3DH_MISO 3
#define LIS3DH_MOSI 4
// Used for hardware & software SPI
#define LIS3DH_CS 2



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
// Adafruit_LIS3DH lis = Adafruit_LIS3DH();
// Adafruit_LIS3DH lis  = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

#define SCREEN_BG COL(0x000)
#define SCREEN_FG COL(0xFF0)
#define BTN_BG    COL(0x330)
#define BTN_FG    COL(0xFF0)
#define BTN_ON_BG COL(0x990)
#define BTN_ON_FG COL(0xFF0)

// Size of time
#define BIG   true
#define SMALL false

// How to draw buttons
#define FILLED   true
#define UNFILLED false

#define NO_BTN 4


// State -----------------------

// Player colors
uint16_t  msgFG[] = { 0, 0, 0, 0 };
uint16_t  msgBG[] = { COL(0xFFF), COL(0x0FF)
                    , COL(0xF00), COL(0x0F0) };


// Player times
unsigned long ptime[] = { 0,0,0,0 };

// Last time we checked the time, or 0 if paused.
unsigned long start_millis = 0;

uint8_t btn_down = NO_BTN; // 0-3, that button was pressed



// Get the location where we touched.
static inline
void getPixelPoint(uint16_t &x, uint16_t &y) {
  uint8_t z;
  ts.readData(&x,&y,&z);
  x = map(x, 150, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
}

// Draw a button
static inline
void drawButton(int x, bool on) {
  uint8_t r = tft.getRotation();
  tft.setRotation(0);
  if (on) {
    tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_ON_BG);
  } else {
    tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_BG);
  }
  tft.setRotation(r);
}

// Compute offsets to skip over the button bar,
// depending on screen orientation.
static inline
void screenOffset (uint8_t r, uint16_t &x, uint16_t &y) {
  x = (r == 3) ? 80 : 0;
  y = (r == 2) ? 80 : 0;
}




static
void sayTime(bool big) {
  uint16_t xx, yy;
  uint8_t r = tft.getRotation();
  char msg[9];
  screenOffset(r,xx,yy);
  if (big) {
    tft.setTextSize(5);
    tft.setCursor(xx + 45, yy + 100);
  } else {
    tft.setTextSize(3);
    tft.setCursor(xx + 80, yy + 10);
  }
  tft.setTextColor(msgFG[r], msgBG[r]);
  Time(ptime[r]).render(msg);
  tft.print(msg + 3);
}

static
void sayPaused() {
  uint16_t xx, yy;
  screenOffset(tft.getRotation(),xx,yy);

  if (start_millis == 0) {
    tft.setTextSize(3);
    tft.setTextColor(SCREEN_FG, SCREEN_BG);
    tft.setCursor(xx + 70, yy + 160);
    tft.print(F("Paused"));
  } else {
    tft.fillRect(xx + 40, yy + 140, 160, 60, SCREEN_BG);
  }
}


static inline
void drawBg(void) {
  const uint16_t w = 160, h = 40;
  const uint16_t x = 40, y = 0;

  tft.setRotation(0);
  tft.fillRect(40,40,160,160,SCREEN_BG);

  for (int i = 0; i < 4; ++i) {
    uint16_t xx, yy;
    tft.setRotation(i);
    screenOffset(i,xx,yy);
    tft.fillRect(xx + x,yy + y,w,h,msgBG[i]);
  }

}


static inline
void activate(uint8_t r) {
  uint16_t xx;
  uint16_t yy;

  // Say the total time for the current player
  uint8_t rold = tft.getRotation();
  screenOffset(rold, xx, yy);
  sayTime(SMALL);

  // Clear the screen.  We flip to the other side, so that the slow
  // drawing gives us a nice "roll up" effect.
  rold = (rold + 2) % 4;
  tft.setRotation(rold);
  screenOffset(rold, xx,yy);
  tft.fillRect(xx + 40, yy + 40, 160, 160, SCREEN_BG);

  // Now switch to the new side.
  tft.setRotation(r);
  screenOffset(r, xx, yy);
  tft.fillRect(xx + 40, yy, 160, 140, msgBG[r]);
  sayTime(BIG);
  if (start_millis != 0) start_millis = millis();
  sayPaused();
}


/* ------------------------------------------------ */
/* Event handling */


static inline
void onScreen(void) {
  if (start_millis == 0) {
    start_millis = millis();
  } else {
    uint8_t p = tft.getRotation();
    ptime[p] += millis() - start_millis;
    start_millis = 0;
  }
  sayPaused();
}

static inline
void onButtonDown(int x) {
  drawButton(x,FILLED);
  btn_down = x;
}

static inline
void onDown(uint16_t x, uint16_t y) {
  if (y < 240) onScreen();
  else onButtonDown(x / 80);
}

static inline
void onUp(void) {
  switch (btn_down) {
    case 0:
    case 1:
    case 2:
      drawButton(btn_down,UNFILLED);
      btn_down = NO_BTN;
      activate ((tft.getRotation() + 1) % 4);
  }
}


void setup() {
  tft.begin();
  ts.begin();
  tft.fillScreen(SCREEN_BG);
  drawBg();
  for (int i = 0; i < 3; ++i) drawButton(i,UNFILLED);

}


void loop(void) {
  static bool done;
  uint16_t x, y;

  if (start_millis != 0) {
    uint8_t p = tft.getRotation();
    unsigned long new_start = millis();
    ptime[p] += (new_start - start_millis);
    start_millis = new_start;
    sayTime(BIG);
  }

  if (ts.touched()) {
    if (ts.bufferEmpty()) return;
    getPixelPoint(x,y);
    if (done) return;
    onDown(x,y);
    done = true;
  } else {
    while (!ts.bufferEmpty()) getPixelPoint(x,y);
    if (done) onUp();
    done = false;
  }

}


