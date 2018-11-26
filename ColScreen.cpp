#include "Screen.h"
#include "Globals.h"
#include "Menu.h"

#define COL_SELECTED COL(0xFFF)
#define NOT_SELECTED 10
#define NO_OWNER 4

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

static
uint16_t getBgCol(uint8_t i) {
  return pgm_read_word(colors + i);
}

static
uint16_t getFgCol(uint8_t i) {
  return pgm_read_word(colors_fg + i);
}





static
void drawColButton(int i, bool on) {
  int x = 80 * (i % 3), y = 80 * (i / 3);
  uint16_t bg_col = getBgCol(i);
  uint16_t fg_col = getFgCol(i);

  tft.fillRect(x, y, 80, 80, SCREEN_BG);
  tft.fillRoundRect(5 + x, 5 + 5 + y, 60, 60, 5, bg_col);
  tft.setCursor(20 + x, 35 + y);
  tft.setTextSize(1);
  tft.setTextColor(fg_col,bg_col);
  tft.print(F("00:00"));
  if (on) {
    tft.drawRoundRect(x, 5 + y, 70, 70, 5, COL_SELECTED);
  }

}




class ColScreen
  : public Screen, public Menu {

  uint8_t owner[9]; // who owns a color
  uint8_t slots[4]; // what is selected for each slot
  int next_p;       // which slot to fill next

public:
  ColScreen()
    : Menu()
    , next_p(0)
  { for (int i = 0; i < 9; ++i) owner[i] = NO_OWNER;
    for (int i = 0; i < 4; ++i) slots[i] = NOT_SELECTED;
  }

  void setup(void) {
    tft.setRotation(0);
    for (int i = 0; i < 9; ++i)
      drawColButton(i, (owner[i] == NO_OWNER) ? BTN_INACTIVE : BTN_ACTIVE);

    drawMenu();
  }

  void onDown(uint16_t x, uint16_t y) {
    if (onMenuDown(x,y)) return;
    x /= 80;
    y /= 80;
    int i = 3 * y + x;
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

  void onUp(void) { onMenuUp(); }

  void menuAction(uint8_t i) {
    switchScreen(ScrClock);
  }

private:
  void nextSlot(void) {
    switch(next_p) {
      case 0: next_p = 2; return;
      case 1: next_p = 3; return;
      case 2: next_p = 1; return;
      case 3: next_p = 0; return;
    }
  }

  void fillSlot(uint8_t col) {
    if (next_p > 3) return;
    drawColButton(col, BTN_ACTIVE);
    owner[col] = next_p;
    msgBG[next_p] = getBgCol(col);
    msgFG[next_p] = getFgCol(col);
    slots[next_p] = col;
  }

  void freeSlot(uint8_t col) {
    if (col > 9) return;
    uint8_t i = owner[col];
    if (i == NO_OWNER) return;
    drawColButton(col, BTN_INACTIVE);
    msgBG[i] = msgFG[i] = SCREEN_BG;
    slots[i] = NOT_SELECTED;
    owner[col] = NO_OWNER;
    next_p = i;
  }
};
