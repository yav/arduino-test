#ifndef __MENU_H__
#define __MENU_H__

#define BTN_BG    COL(0x330)
#define BTN_FG    COL(0xFF0)
#define BTN_ON_BG COL(0x990)
#define BTN_ON_FG COL(0xFF0)

#define BTN_INACTIVE false
#define BTN_ACTIVE   true
#define NO_BTN       4

// Compute offsets to skip over the button bar,
// depending on screen orientation.
inline
void screenOffset (uint8_t r, uint16_t &x, uint16_t &y) {
  x = (r == 3) ? 80 : 0;
  y = (r == 2) ? 80 : 0;
}

class Menu {
  uint8_t btn_down;

public:
  Menu(void) : btn_down(NO_BTN) {}

protected:
  void drawMenu(void) {
    for (int i = 0; i < 3; ++i) drawButton(i,BTN_INACTIVE);
  }

  bool onMenuDown(uint16_t x, uint16_t y) {
    if (y < 240) return false;
    x /= 80;
    drawButton(x,BTN_ACTIVE);
    btn_down = x;
    return true;
  }

  void onMenuUp(void) {
    if (btn_down == NO_BTN) return;
    menuAction(btn_down);
    drawButton(btn_down,BTN_INACTIVE);
    btn_down = NO_BTN;
  }

  virtual void menuAction(uint8_t btn) = 0;

private:
  static
  void drawButton(int x, bool on) {
    tft.setRotation(0);
    if (on) {
      tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_ON_BG);
    } else {
      tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_BG);
    }
  }



};

#endif
