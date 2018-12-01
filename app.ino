#include <SPI.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <avr/pgmspace.h>

#include "Globals.h"
#include "Color.h"
#include "Screen.h"

#include "ColScreen.h"
#include "ClockScreen.h"

#define STMPE_CS 8
#define TFT_DC   9
#define TFT_CS  10

#define LIS3DH_CLK 3
#define LIS3DH_MOSI 5
#define LIS3DH_MISO 6
#define LIS3DH_CS 7



Adafruit_ILI9341 tft  = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts  = Adafruit_STMPE610(STMPE_CS);
Adafruit_LIS3DH lis = Adafruit_LIS3DH
                        (LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

// Player colors
uint16_t  msgFG[] = { SCREEN_BG, SCREEN_BG, SCREEN_BG, SCREEN_BG };
uint16_t  msgBG[] = { SCREEN_BG, SCREEN_BG, SCREEN_BG, SCREEN_BG };


ClockScreen clockScreen;
ColScreen colScreen;
Screen *screen;

uint8_t ori;
#define DIR_UNKNOWN 4

void switchScreen(Screens x) {
  switch (x) {
    case ScrClock: screen = &clockScreen; break;
    case ScrColor: screen = &colScreen; break;
  }
  tft.fillScreen(SCREEN_BG);
  screen->setup(ori);
}


// Get the location where we touched.
static inline
void getPixelPoint(uint16_t &x, uint16_t &y) {
  uint8_t z;
  ts.readData(&x,&y,&z);
  x = map(x, 150, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
}

static inline
bool dirActive(int16_t x) { return (x < -10000 || x > 10000); }


static inline
uint8_t orientation() {
  lis.read();
  if (dirActive(lis.z)) return DIR_UNKNOWN;
  if (lis.y > 10000)  return dirActive(lis.x) ? DIR_UNKNOWN : 1;
  if (lis.y < -10000) return dirActive(lis.x) ? DIR_UNKNOWN : 3;
  if (lis.x < -10000) return dirActive(lis.y) ? DIR_UNKNOWN : 0;
  if (lis.x >  10000) return dirActive(lis.y) ? DIR_UNKNOWN : 2;
  return DIR_UNKNOWN;
}



void setup() {
  Serial.begin(9600);
  Serial.println(F("HELLO!"));
  tft.begin();
  ts.begin();
  lis.begin();
  lis.setRange(LIS3DH_RANGE_2_G);

  ori = orientation();
  switchScreen(ScrColor);
}


void loop(void) {
  static bool done;
  static unsigned long rot_start;
  uint16_t x, y;

  screen->update();

  uint8_t new_ori = orientation();
  if (new_ori != ori) {
      rot_start = millis();
      ori = new_ori;
  } else {
    if (rot_start > 0 && (millis () - rot_start > 500)) {
      screen->rotated(ori);
      rot_start = 0;
    }
  }

  if (ts.touched()) {
    if (ts.bufferEmpty()) return;
    getPixelPoint(x,y);
    if (done) return;
    screen->onDown(x,y);
    done = true;
  } else {
    while (!ts.bufferEmpty()) getPixelPoint(x,y);
    if (done) screen->onUp();
    done = false;
  }

}


