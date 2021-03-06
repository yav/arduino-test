#ifndef __MENU_H__
#define __MENU_H__

#define BTN_BG    COL(0x330)
#define BTN_FG    COL(0xFF0)
#define BTN_ON_BG COL(0x990)
#define BTN_ON_FG COL(0xFF0)

#define BTN_INACTIVE false
#define BTN_ACTIVE   true
#define NO_BTN       4

typedef char const* const* MenuLabels;

class Menu {
  uint8_t btn_down;

protected:
  Menu(void)
    : btn_down(NO_BTN) {}

  void drawMenu(void) {
    for (int i = 0; i < 3; ++i) drawButton(i,BTN_INACTIVE);
  }

  bool onMenuDown(uint16_t x, uint16_t y) {
    if (y < 240) return false;
    x /= 80;
    if (!buttonUsed(x)) return false;
    drawButton(x,BTN_ACTIVE);
    btn_down = x;
    return true;
  }

  void onMenuUp(void) {
    uint16_t b = btn_down;
    if (btn_down == NO_BTN) return;
    drawButton(btn_down,BTN_INACTIVE);
    btn_down = NO_BTN;
    menuAction(b);
  }

  virtual void menuAction(uint8_t btn) {
  }

  virtual bool buttonUsed(uint8_t btn) {
    return true;
  }

  virtual MenuLabels menuLabels(void) {
    return NULL;
  }

  // Compute offsets to skip over the button bar,
  // depending on screen orientation.
  static inline
  void screenOffset (uint8_t r, uint16_t &x, uint16_t &y) {
    x = (r == 3) ? 80 : 0;
    y = (r == 2) ? 80 : 0;
  }


private:

  void drawButton(int i, bool on) {
    if (!buttonUsed(i)) return;

    const uint16_t x = 80 * i, y = 240;

    tft.setRotation(0);
    tft.setTextSize(1);
    if (on) {
      tft.setTextColor(BTN_ON_FG, BTN_ON_BG);
      tft.fillRoundRect(x + 5, y + 5, 70, 70, 5, BTN_ON_BG);
    } else {
      tft.setTextColor(BTN_FG, BTN_BG);
      tft.fillRoundRect(x + 5, y + 5, 70, 70, 5, BTN_BG);
    }

    MenuLabels labs = menuLabels();
    if (!labs) return;

    char buf[20];
    strncpy_P(buf, (char const*)pgm_read_word(labs + i), 20);
    buf[19] = 0;

    int16_t bx,by;
    uint16_t bw,bh;
    tft.getTextBounds(buf,x,y,&bx,&by,&bw,&bh);
    tft.setCursor(x + 40 - (bw/2), 35 + y);
    tft.print(buf);
  }
};

#endif
