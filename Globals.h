#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <Adafruit_ILI9341.h>
#include <inttypes.h>
#include "Color.h"

#define SCREEN_BG COL(0x000)
#define SCREEN_FG COL(0xFF0)

extern Adafruit_ILI9341 tft;
extern uint16_t msgFG[4];
extern uint16_t msgBG[4];

enum Screens { ScrClock, ScrColor };
void switchScreen(Screens x);

#endif
