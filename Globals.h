#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <Adafruit_ILI9341.h>
#include <inttypes.h>
#include "Color.h"

// #define SCREEN_BG COL(0x033)
#define SCREEN_BG COL(0x000)
#define SCREEN_FG COL(0xFF0)

extern Adafruit_ILI9341 tft;
extern uint16_t msgFG[4];
extern uint16_t msgBG[4];

enum Screens { ScrClock, ScrColor };
void switchScreen(Screens x);


static inline
void gradRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                                                 uint16_t c1, uint16_t c2) {
  uint16_t w1 = w/5;
  uint16_t w2 = w - w1;

  int16_t r1 = COL565_R(c1) << 9;
  int16_t g1 = COL565_G(c1) << 9;
  int16_t b1 = COL565_B(c1) << 9;

  int16_t r2 = COL565_R(c2) << 9;
  int16_t g2 = COL565_G(c2) << 9;
  int16_t b2 = COL565_B(c2) << 9;

  int16_t step_r = (r2 - r1) / (int16_t)w1;
  int16_t step_g = (g2 - g1) / (int16_t)w1;
  int16_t step_b = (b2 - b1) / (int16_t)w1;

  tft.startWrite();
  tft.setAddrWindow(x,y,w,h);
  for (uint16_t py = 0; py < h; ++py) {
    uint16_t px;
    int16_t r = r1;
    int16_t g = g1;
    int16_t b = b1;
    for (px = 0; px < w1; ++px) {
      tft.writePixel(COL565(r >> 9,g >> 9,b >> 9));
      r += step_r;
      g += step_g;
      b += step_b;
    }
    for (; px < w2; ++px) {
      tft.writePixel(COL565(r >> 9,g >> 9,b >> 9));
    }
    for (; px < w; ++px) {
      tft.writePixel(COL565(r >> 9,g >> 9,b >> 9));
      r -= step_r;
      g -= step_g;
      b -= step_b;
    }
  }
  tft.endWrite();
 }




#endif
