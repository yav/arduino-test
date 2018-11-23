#include <SPI.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#include "Color.h"

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
uint8_t z;

static inline
void getPoint(void) {
  uint16_t t;
  ts.readData(&x,&y,&z);
  x = map(x, 150, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
  switch (tft.getRotation()) {
    case 1: t = x; x = y; y = 245 - t; break;
    case 2: x = 245 - x; y = 325 - y; break;
    case 3: y = 325 - y; t = x; x = y; y = t;
  }
}

static const int w = 80;

static inline
void drawSquare(int x, int y, uint16_t c) {
  tft.drawRect(1 + w*x, 1 + w*y, w-2, w-2, c);
}

static inline
void drawBG(void) {
  for (int x = 0; x < 240/w; ++x) {
    for (int y = 0; y < 320/w; ++y) {
      drawSquare(x,y,COL444(0x999));
    }
  }
}

char msg[64];


static inline
void clearMsg(void) {
  int16_t x,y;
  uint16_t w,h;
  tft.getTextBounds(msg, 0, 0, &x, &y, &w, &h);
  tft.fillRect(x,y,w,h,COL444(0x000));
  tft.setCursor(0,0);
  msg[0] = 0;
}

static inline
void say(unsigned long t) {
  clearMsg();
  snprintf(msg,sizeof(msg),"%lu",t);
  tft.print(msg);
}


void setup() {
  tft.begin();
  ts.begin();
  tft.fillScreen(COL444(0x000));
  tft.setTextColor(COL444(0x0F0));
  tft.setTextSize(3);
  tft.setRotation(3);
}

uint8_t sx, sy;

static inline
void onDown(void) {
  sx = x / w;
  sy = y / w;
  drawSquare(sx, sy, COL444(0xF00));
}

static inline
void onUp(void) {
  drawSquare(sx, sy, COL444(0xFFF));
}

void loop(void) {
  static bool done;

  say(millis());

  if (ts.touched()) {
    if (ts.bufferEmpty()) return;
    getPoint();
    if (done) return;
    onDown();
    done = true;
  } else {
    if (done) onUp();
    done = false;
  }

}


