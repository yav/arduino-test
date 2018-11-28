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

// Used for software SPI
#define LIS3DH_CLK 5
#define LIS3DH_MISO 3
#define LIS3DH_MOSI 4
// Used for hardware & software SPI
#define LIS3DH_CS 2



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// Player colors
uint16_t  msgFG[] = { SCREEN_BG, SCREEN_BG, SCREEN_BG, SCREEN_BG };
uint16_t  msgBG[] = { SCREEN_BG, SCREEN_BG, SCREEN_BG, SCREEN_BG };


ClockScreen clockScreen;
ColScreen colScreen;
Screen *screen;

void switchScreen(Screens x) {
  switch (x) {
    case ScrClock: screen = &clockScreen; break;
    case ScrColor: screen = &colScreen; break;
  }
  tft.fillScreen(SCREEN_BG);
  screen->setup();
}


// Get the location where we touched.
static inline
void getPixelPoint(uint16_t &x, uint16_t &y) {
  uint8_t z;
  ts.readData(&x,&y,&z);
  x = map(x, 150, 3800, 0, 245);
  y = map(y, 180, 3800, 0, 325);
}




void setup() {
  tft.begin();
  ts.begin();
  tft.fillScreen(SCREEN_BG);
  // screen = &clockScreen;
  screen = &colScreen;
  screen->setup();
  screen->rotated(0);

}


void loop(void) {
  static bool done;
  uint16_t x, y;

  screen->update();

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


