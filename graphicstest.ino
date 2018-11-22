#include <SPI.h>
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



static inline
void getPoint(uint16_t &x, uint16_t &y, uint8_t &z) {
  ts.readData(&x,&y,&z);
  x = map(x, 180, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
}



void setup() {
  tft.begin();
  ts.begin();
  tft.fillScreen(COL444(0x000));
  tft.setTextColor(COL444(0xFFF));
  tft.setTextSize(3);
}

void loop(void) {
  int done = 0;
  uint16_t x,y;
  uint8_t z;

  while (ts.touched()) {
    getPoint(x,y,z);

    if (!done) {
      tft.fillScreen(COL444(0x000));
      tft.setCursor(0,0);
      tft.print(x); tft.print(", "); tft.println(y);
      tft.drawCircle(x,y,5,COL444(0x0FF));
      done = 1;
    }
  }
  while(!ts.bufferEmpty()) getPoint(x,y,z);

  tft.fillScreen(COL444(0xF00));

 delay(200);
}


