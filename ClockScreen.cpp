#include <Adafruit_ILI9341.h>

#include "Globals.h"
#include "Screen.h"
#include "YTime.h"
#include "Color.h"
#include "Menu.h"

#define BIG      true
#define SMALL    false
#define PAUSED   0

const char clk_btn1[] PROGMEM = "Rotate";
const char clk_btn2[] PROGMEM = "";
const char clk_btn3[] PROGMEM = "Color";
const char* const clk_btns[] PROGMEM = { clk_btn1, clk_btn2, clk_btn3 };

class ClockScreen
  : public Screen, public Menu
{

  unsigned long start_millis;
  unsigned long ptime[4];
  uint8_t orient;

  public:
    ClockScreen(void)
      : Menu()
      , start_millis(PAUSED)
      , ptime { 0, 0, 0, 0 }
      , orient(0)

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
      sayTime(i,SMALL);
    }

    drawMenu();
    activate(orient);
  }


  void update(void) {
    if (start_millis != 0) {
      unsigned long new_start = millis();
      ptime[orient] += (new_start - start_millis);
      start_millis = new_start;
      sayTime(BIG);
    }
  }

  void rotated(uint8_t d) {
    setOrient(d);
    drawMenu();
    activate(d);
  }


  void onDown(uint16_t x, uint16_t y) {
    if (onMenuDown(x,y)) return;
    pause();
  }

  void onUp(void) { onMenuUp(); }

  private:
  const char * const* menuLabels(void) { return clk_btns; }

  void menuAction(uint8_t i) {
    switch (i) {
      case 0: rotated((orient + 1) % 4); break;
      case 2:
        pause();
        switchScreen(ScrColor);
        break;
    }
  }

    void pause (void) {
      if (start_millis == PAUSED) {
        start_millis = millis();
      } else {
        ptime[orient] += millis() - start_millis;
        start_millis = 0;
      }
      sayPaused();
    }


  void sayTime(bool big) { sayTime(orient,big); }

  void sayTime(uint8_t r, bool big) {
    uint16_t xx, yy;
    char msg[9];
    tft.setRotation(r);
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
    screenOffset(orient,xx,yy);
    tft.setRotation(orient);
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

    if (r != orient) {
      // Say the total time for the current player
      uint8_t rold = orient;
      screenOffset(rold, xx, yy);
      tft.setRotation(rold);
      sayTime(SMALL);

      // Clear the screen.  We flip to the other side, so that the slow
      // drawing gives us a nice "roll up" effect.
      rold = (rold + 2) % 4;
      tft.setRotation(rold);
      screenOffset(rold, xx,yy);
      tft.fillRect(xx + 40, yy + 40, 160, 160, SCREEN_BG);
    }

    // Now switch to the new side.
    orient = r;
    tft.setRotation(r);
    screenOffset(r, xx, yy);
    tft.fillRect(xx + 40, yy, 160, 140, msgBG[r]);
    sayTime(BIG);
    if (start_millis != 0) start_millis = millis();
    sayPaused();
  }

};






