#include <Adafruit_ILI9341.h>

#include "Screen.h"
#include "YTime.h"
#include "Color.h"

#define SCREEN_BG COL(0x000)
#define SCREEN_FG COL(0xFF0)
#define BTN_BG    COL(0x330)
#define BTN_FG    COL(0xFF0)
#define BTN_ON_BG COL(0x990)
#define BTN_ON_FG COL(0xFF0)

#define BIG      true
#define SMALL    false
#define FILLED   true
#define UNFILLED false
#define NO_BTN   4
#define PAUSED   0






extern Adafruit_ILI9341 tft;
extern uint16_t msgFG[4];
extern uint16_t msgBG[4];

// Compute offsets to skip over the button bar,
// depending on screen orientation.
static inline
void screenOffset (uint8_t r, uint16_t &x, uint16_t &y) {
  x = (r == 3) ? 80 : 0;
  y = (r == 2) ? 80 : 0;
}


static
void drawButton(int x, bool on) {
  uint8_t r = tft.getRotation();
  tft.setRotation(0);
  if (on) {
    tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_ON_BG);
  } else {
    tft.fillRoundRect(80 * x, 245, 70, 70, 5, BTN_BG);
  }
  tft.setRotation(r);
}



class ClockScreen
  : public Screen {

  uint8_t btn_down;
  unsigned long start_millis;
  unsigned long ptime[4];

  public:
    ClockScreen(void)
      : btn_down(NO_BTN)
      , start_millis(PAUSED)
      , ptime { 0, 0, 0, 0 }

      {}

  void setup(void) {
    const uint16_t w = 160, h = 40;
    const uint16_t x = 40, y = 0;

    tft.setRotation(0);
    tft.fillRect(40,40,160,160,SCREEN_BG);

    for (int i = 0; i < 4; ++i) {
      uint16_t xx, yy;
      tft.setRotation(i);
      screenOffset(i,xx,yy);
      tft.fillRect(xx + x,yy + y,w,h,msgBG[i]);
    }

    for (int i = 0; i < 3; ++i) drawButton(i,UNFILLED);
  }


  void update(void) {
    if (start_millis != 0) {
      uint8_t p = tft.getRotation();
      unsigned long new_start = millis();
      ptime[p] += (new_start - start_millis);
      start_millis = new_start;
      sayTime(BIG);
    }
  }

  void rotated(uint8_t d) {
    activate(d);
  }


  void onDown(uint16_t x, uint16_t y) {
    if (y < 240) pause();
    else {
      x /= 80;
      drawButton(x,FILLED);
      btn_down = x;
    }
  }


  void onUp(void) {
    switch (btn_down) {
      case 0:
      case 1:
      case 2:
        drawButton(btn_down,UNFILLED);
        btn_down = NO_BTN;
        activate ((tft.getRotation() + 1) % 4);
    }
  }

  private:
    void pause (void) {
      if (start_millis == PAUSED) {
        start_millis = millis();
      } else {
        uint8_t p = tft.getRotation();
        ptime[p] += millis() - start_millis;
        start_millis = 0;
      }
      sayPaused();
    }



  void sayTime(bool big) {
    uint16_t xx, yy;
    uint8_t r = tft.getRotation();
    char msg[9];
    screenOffset(r,xx,yy);
    if (big) {
      tft.setTextSize(5);
      tft.setCursor(xx + 45, yy + 100);
    } else {
      tft.setTextSize(3);
      tft.setCursor(xx + 80, yy + 10);
    }
    tft.setTextColor(msgFG[r], msgBG[r]);
    Time(ptime[r]).render(msg);
    tft.print(msg + 3);
  }

  void sayPaused() {
    uint16_t xx, yy;
    screenOffset(tft.getRotation(),xx,yy);

    if (start_millis == 0) {
      tft.setTextSize(3);
      tft.setTextColor(SCREEN_FG, SCREEN_BG);
      tft.setCursor(xx + 70, yy + 160);
      tft.print(F("Paused"));
    } else {
      tft.fillRect(xx + 40, yy + 140, 160, 60, SCREEN_BG);
    }
  }


  void activate(uint8_t r) {
    uint16_t xx;
    uint16_t yy;

    // Say the total time for the current player
    uint8_t rold = tft.getRotation();
    screenOffset(rold, xx, yy);
    sayTime(SMALL);

    // Clear the screen.  We flip to the other side, so that the slow
    // drawing gives us a nice "roll up" effect.
    rold = (rold + 2) % 4;
    tft.setRotation(rold);
    screenOffset(rold, xx,yy);
    tft.fillRect(xx + 40, yy + 40, 160, 160, SCREEN_BG);

    // Now switch to the new side.
    tft.setRotation(r);
    screenOffset(r, xx, yy);
    tft.fillRect(xx + 40, yy, 160, 140, msgBG[r]);
    sayTime(BIG);
    if (start_millis != 0) start_millis = millis();
    sayPaused();
  }

};






