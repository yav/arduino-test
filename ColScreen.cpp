#include "Screen.h"
#include "Globals.h"
#include "Menu.h"
#include "ColScreen.h"

#define COL_SELECTED COL(0xFFF)
#define NOT_SELECTED 10
#define NO_OWNER 4

namespace ColScreenNS {
  const uint_fast16_t colors[9] PROGMEM =
    { COL(0xF00), COL(0x090), COL(0x00F)
    , COL(0xFF0), COL(0x606), COL(0xF60)
    , COL(0x111), COL(0x999), COL(0xFFF)
    };

  const uint_fast16_t colors_fg[9] PROGMEM =
    { COL(0xFFF), COL(0xFFF), COL(0xFFF)
    , COL(0x000), COL(0xFFF), COL(0x000)
    , COL(0xCCC), COL(0x000), COL(0x333)
    };

  uint16_t getBgCol(uint8_t i) {
    return pgm_read_word(colors + i);
  }

  uint16_t getFgCol(uint8_t i) {
    return pgm_read_word(colors_fg + i);
  }

  const char col_btn1[] PROGMEM = "Roate";
  const char col_btn2[] PROGMEM = "";
  const char col_btn3[] PROGMEM = "Clock";
  char const* const col_btns[] PROGMEM = { col_btn1, col_btn2, col_btn3 };

  void drawColButton(uint8_t i, bool on) {
    uint16_t x = 80 * (i % 3), y = 80 * (i / 3);
    uint16_t bg_col = getBgCol(i);
    // uint16_t fg_col = getFgCol(i);

    tft.setRotation(0);
    tft.fillRect(x, y, 80, 80, SCREEN_BG);
    tft.fillRoundRect(10 + x, 5 + 5 + y, 60, 60, 5, bg_col);
    tft.setCursor(25 + x, 35 + y);
    if (on) {
      tft.drawRoundRect(5 + x, 5 + y, 70, 70, 5, COL_SELECTED);
    }
  }
}


ColScreen::ColScreen() : Menu(), next_p(0) {
  for (uint8_t i = 0; i < 9; ++i) owner[i] = NO_OWNER;
  for (uint8_t i = 0; i < 4; ++i) slots[i] = NOT_SELECTED;
}


void ColScreen::setup() {
  tft.setRotation(0);
  for (uint8_t i = 0; i < 9; ++i)
    ColScreenNS::drawColButton(i, (owner[i] == NO_OWNER) ? BTN_INACTIVE : BTN_ACTIVE);
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
  setOrient(d);
  drawMenu();
}

char const* const* ColScreen::menuLabels() {
  return ColScreenNS::col_btns;
}

void ColScreen::menuAction(uint8_t i) {
  switch(i) {
    case 0: rotated((getOrient() + 1) % 4); break;
    case 2: switchScreen(ScrClock); break;
  }
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
  ColScreenNS::drawColButton(col, BTN_ACTIVE);
  owner[col] = next_p;
  msgBG[next_p] = ColScreenNS::getBgCol(col);
  msgFG[next_p] = ColScreenNS::getFgCol(col);
  slots[next_p] = col;
}

void ColScreen::freeSlot(uint8_t col) {
  if (col > 9) return;
  uint8_t i = owner[col];
  if (i == NO_OWNER) return;

  ColScreenNS::drawColButton(col, BTN_INACTIVE);
  msgBG[i]    = msgFG[i] = SCREEN_BG;
  slots[i]    = NOT_SELECTED;
  owner[col]  = NO_OWNER;
  next_p      = i;
}

