#include "Screen.h"
#include "Globals.h"
#include "Menu.h"
#include "ColScreen.h"

#define COL_SELECTED COL(0xFFF)
#define NOT_SELECTED 1
#define NO_OWNER 4

namespace ColScreenNS {
  const uint_fast16_t colors[] PROGMEM =
    { COL(0xF00), COL(0x090), COL(0x00F)
    , COL(0xFF0), COL(0x606), COL(0xF60)
    , COL(0x111), COL(0x999), COL(0xFFF)
    , COL(0xF6C)
    };

  const uint_fast16_t colors_fg[] PROGMEM =
    { COL(0xFFF), COL(0xFFF), COL(0xFFF)
    , COL(0x000), COL(0xFFF), COL(0x000)
    , COL(0xFFF), COL(0x000), COL(0x000)
    , COL(0x000)
    };

  uint16_t getBgCol(uint8_t i) {
    return pgm_read_word(colors + i);
  }

  uint16_t getFgCol(uint8_t i) {
    return pgm_read_word(colors_fg + i);
  }

  const char col_btn1[] PROGMEM = "";
  const char col_btn2[] PROGMEM = "";
  const char col_btn3[] PROGMEM = "Clock";
  char const* const col_btns[] PROGMEM = { col_btn1, col_btn2, col_btn3 };

  void tri(uint8_t i, uint8_t dir, int16_t x, int16_t y) {
    uint16_t c = getFgCol(i);
    switch (dir) {
      case 0: tft.fillTriangle(x-5,y+5,x,y-10,x+5,y+5,c); break;
      case 1: tft.fillTriangle(x-5,y-5,x+10,y,x-5,y+5,c); break;
      case 2: tft.fillTriangle(x-5,y-5,x,y+10,x+5,y-5,c); break;
      case 3: tft.fillTriangle(x+5,y+5,x-10,y,x+5,y-5,c); break;
    }
  }

  void drawColButton(uint8_t i, uint8_t owner) {
    uint16_t x = 80 * (i % 3), y = 80 * (i / 3);
    uint16_t bg_col = getBgCol(i);

    tft.setRotation(0);
    tft.fillRect(x, y, 80, 80, SCREEN_BG);
    tft.fillRoundRect(10 + x, 5 + 5 + y, 60, 60, 5, bg_col);
    if (owner != NO_OWNER) {
      tft.drawRoundRect(5 + x, 5 + y, 70, 70, 5, COL_SELECTED);
      tri(i, owner, 40 + x, 40 + y);
    }
  }

}


ColScreen::ColScreen() : Menu(), next_p(0) {
  for (uint8_t i = 0; i < 10; ++i) owner[i] = NO_OWNER;
  for (uint8_t i = 0; i < 4; ++i) slots[i] = NOT_SELECTED;
}


void ColScreen::setup(uint8_t d) {
  tft.setRotation(0);
  for (uint8_t i = 0; i < 10; ++i)
    ColScreenNS::drawColButton(i, owner[i]);
    drawMenu();
}


void ColScreen::onDown(uint16_t x, uint16_t y) {
  if (onMenuDown(x,y)) return;
  x /= 80;
  y /= 80;
  uint8_t i = 3 * y + x;
  if (owner[i] == NO_OWNER) {
    fillSlot(i);

    for (i = 0; i < 4; ++i) {
      nextSlot();
      if (slots[next_p] == NOT_SELECTED) return;
    }
    next_p = NO_OWNER;

  } else {
    freeSlot(i);
  }
}


void ColScreen::onUp() {
  onMenuUp();
}


void ColScreen::rotated(uint8_t d) {
  drawMenu();
}

MenuLabels ColScreen::menuLabels() {
  return ColScreenNS::col_btns;
}

void ColScreen::menuAction(uint8_t i) {
  switch(i) {
    case 2: switchScreen(ScrClock); break;
  }
}

bool ColScreen::buttonUsed(uint8_t btn) {
  return btn > 0;
}

void ColScreen::nextSlot() {
  switch(next_p) {
    case 0: next_p = 2; return;
    case 1: next_p = 3; return;
    case 2: next_p = 1; return;
    case 3: next_p = 0; return;
  }
}

void ColScreen::fillSlot(uint8_t col) {
  if (next_p > 3) return;
  ColScreenNS::drawColButton(col, next_p);
  owner[col] = next_p;
  msgBG[next_p] = ColScreenNS::getBgCol(col);
  msgFG[next_p] = ColScreenNS::getFgCol(col);
  slots[next_p] = col;
}

void ColScreen::freeSlot(uint8_t col) {
  if (col >= 10) return;
  uint8_t i = owner[col];
  if (i == NO_OWNER) return;

  ColScreenNS::drawColButton(col, NO_OWNER);
  msgBG[i]    = msgFG[i] = SCREEN_BG;
  slots[i]    = NOT_SELECTED;
  owner[col]  = NO_OWNER;
  next_p      = i;
}


