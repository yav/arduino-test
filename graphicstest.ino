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


uint16_t x, y;

static inline
void rot(uint16_t maxX, uint16_t maxY, uint16_t &x, uint16_t &y) {
  uint16_t t;
  switch (tft.getRotation()) {
    case 1: t = x; x = y; y = maxX - t; break;
    case 2: x = maxX - x; y = maxY - y; break;
    case 3: t = x; x = maxY - y; y = t;
  }
}

static inline
void unrot(uint16_t maxX, uint16_t maxY, uint16_t &x, uint16_t &y) {
  uint16_t t;
  switch (tft.getRotation()) {
    case 1: t = y; y = x; x = maxX - t; break;
    case 2: x = maxX - x; y = maxY - y; break;
    case 3: t = y; y = maxY - x; x = t;
  }
}

static inline
void getPixelPoint(void) {
  uint8_t z;
  ts.readData(&x,&y,&z);
  x = map(x, 150, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
  rot(245,345,x,y);
}





static const int w = 80;

static inline
void drawSquare(int x, int y, uint16_t c) {
  tft.drawRect(1 + w*x, 1 + w*y, w-2, w-2, c);
}


static inline
void drawSquareFilled(int x, int y, uint16_t c) {
  tft.fillRect(1 + w*x, 1 + w*y, w-2, w-2, c);
}



char msg[9];

uint16_t  msgFG[] = { 0, 0, 0, 0 };
uint16_t  msgBG[] = { COL(0xFFF), COL(0xF00)
                    , COL(0x00F), COL(0xFF0) };



static inline
void clearMsg(void) {
  int16_t x,y;
  uint16_t w,h;
  int r = tft.getRotation();
  tft.getTextBounds(msg + 3, (r == 3) ? 125 : 45
                           , (r == 2) ? 180 : 100
                           , &x, &y, &w, &h);
  tft.fillRect(x,y,w,h,COL(0x000));
  msg[0] = 0;
}


static inline
void sayTime(Time t) {
  clearMsg();
  int r = tft.getRotation();
  tft.setCursor((r == 3) ? 125: 45, (r == 2) ? 180 : 100);
  tft.setTextColor(msgFG[r], msgBG[r]);
  t.render(msg);
  tft.print(msg + 3);
}

static inline
void drawBg(void) {
  uint16_t w = 160, h = 40;
  int i = tft.getRotation();
  uint16_t xx = 0, yy = 0;
  if (i == 1 || i == 3) { uint16_t t = w; w = h; h = t; }
  if (i == 2) { yy = 80; }
  if (i == 3) { xx = 80; }
  static uint16_t xs[] = { 40,200,40,0 };
  static uint16_t ys[] = { 0, 40, 200, 40 };

  tft.fillRect(xx,yy,240,240,COL(0x000));
  tft.fillRect(xx + 40, yy + 40, 160, 100, msgBG[i]);

  i = (4 - i) % 4;
  tft.fillRect(xx + xs[i], yy + ys[i], w, h, msgBG[0]);
  i = (i + 1) % 4;
  tft.fillRect(xx + xs[i], yy + ys[i], h, w, msgBG[1]);
  i = (i + 1) % 4;
  tft.fillRect(xx + xs[i], yy + ys[i], w, h, msgBG[2]);
  i = (i + 1) % 4;
  tft.fillRect(xx + xs[i], yy + ys[i], h, w, msgBG[3]);
}

Time time;

void setup() {
  tft.begin();
  ts.begin();
  tft.fillScreen(COL(0x000));
  tft.setTextColor(COL(0x0F0),COL(0x060));
  tft.setTextSize(5);
  time = Time(millis());
  drawSquare(0,3, COL(0xF00));
  drawSquare(1,3, COL(0x0F0));
  drawSquare(2,3, COL(0x00F));

  drawBg();

}


static
void onScreenAt(int x, int y) {
}

bool rotate;

static
void onButton(int x) {
  switch(x) {
    case 0: rotate = true;
  }
}



static inline
void onDown(void) {
  int sx = x / w;
  int sy = y / w;
  switch (tft.getRotation()) {
    case 0:
      if (sy < 3) onScreenAt(sx,sy);
      else onButton(sx);
      break;
    case 1:
      if (sx < 3) onScreenAt(sx,sy);
      else onButton(2-sy);
      break;
    case 2:
      if (sy > 0) onScreenAt(sx,sy - 1);
      else onButton(2-sx);
      break;
    case 3:
      if (sx > 0) onScreenAt(sx - 1, sy);
      else onButton(sy);
      break;
  }
}

static inline
void onUp(void) {
  if (rotate) {
    clearMsg();
    tft.setRotation((tft.getRotation() + 1) % 4);
    drawBg();
    sayTime(time);
    rotate = false;
  }
}



void loop(void) {
  static bool done;
  Time t = Time(millis());
  if (t != time) {
    sayTime(Time(millis()));
    time = t;
  }

  if (ts.touched()) {
    if (ts.bufferEmpty()) return;
    getPixelPoint();
    if (done) return;
    onDown();
    done = true;
  } else {
    if (done) onUp();
    done = false;
  }

}


