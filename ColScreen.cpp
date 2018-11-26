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
      if (next_p > 3) return;
          drawColButton(i, BTN_ACTIVE);
          owner[i] = next_p;
          msgBG[next_p] = getBgCol(i);
          msgFG[next_p] = getFgCol(i);
          slots[next_p] = i;

          for (next_p = 0; next_p < 4; ++next_p) {
            if (slots[next_p] == NOT_SELECTED) return;
          }
          next_p = NO_OWNER;
    } else {
      drawColButton(i, BTN_INACTIVE);
      next_p = owner[i];
      msgBG[next_p] = SCREEN_BG;
      msgFG[next_p] = SCREEN_BG;
      slots[next_p] = NOT_SELECTED;
      owner[i] = NO_OWNER;
    }
  }

  void onUp(void) { onMenuUp(); }

  void menuAction(uint8_t i) {
    switchScreen(ScrClock);
  }

};
