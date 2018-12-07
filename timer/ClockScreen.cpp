#include <Adafruit_ILI9341.h>

#include "Globals.h"
#include "Screen.h"
#include "YTime.h"
#include "Color.h"
#include "Menu.h"
#include "ClockScreen.h"

#define BIG      true
#define SMALL    false
#define PAUSED   0


namespace ClockScreenNS {

  char const clk_btn1[] PROGMEM = "";
  char const clk_btn2[] PROGMEM = "";
  char const clk_btn3[] PROGMEM = "Color";
  char const* const clk_btns[] PROGMEM = { clk_btn1, clk_btn2, clk_btn3 };

}



ClockScreen::ClockScreen()
  : Menu(), start_millis(PAUSED), ptime { 0, 0, 0, 0 }, orient(0) {}

void ClockScreen::setup(uint8_t dir) {
  const uint16_t w = 160, h = 40;
  const uint16_t x = 40, y = 0;

  tft.setRotation(0);
  tft.fillRect(40,40,160,160,SCREEN_BG);

  for (uint8_t i = 0; i < 4; ++i) {
    uint16_t xx, yy;
    tft.setRotation(i);
    Menu::screenOffset(i,xx,yy);
    gradRect(xx + x,yy + y,w,h,SCREEN_BG,msgBG[i]);
    sayTime(i,SMALL);
  }

  drawMenu();
  if (dir == DIR_UNKNOWN) {
    pause(true);
    sayPaused();
  }
  else activate(dir);
}

void ClockScreen::update() {
  if (start_millis != PAUSED) {
    unsigned long new_start = millis();
    unsigned long elapsed = new_start - start_millis;
    if (elapsed > 1000) {
      ptime[orient] += elapsed;
      start_millis = new_start;
      sayTime(BIG);
    }
  }
}

void ClockScreen::rotated(uint8_t d) {
  if (d == DIR_UNKNOWN) pause(true);
  else {
    drawMenu();
    activate(d);
  }
}


void ClockScreen::onDown(uint16_t x, uint16_t y) {
  onMenuDown(x,y);
}

void ClockScreen::onUp() {
  onMenuUp();
}

bool ClockScreen::buttonUsed(uint8_t b) {
  return b == 2;
}

MenuLabels ClockScreen::menuLabels() {
  return ClockScreenNS::clk_btns;
}

void ClockScreen::menuAction(uint8_t i) {
  switch (i) {

    case 0:
      rotated((orient + 1) % 4);
      break;

    case 2:
      pause(true);
      switchScreen(ScrColor);
      break;
  }
}

void ClockScreen::pause (bool on) {
  if (on) {
    if (start_millis == PAUSED) return;
    ptime[orient] += millis() - start_millis;
    start_millis = PAUSED;
  } else {
    if (start_millis != PAUSED) return;
    start_millis = millis();
  }
  sayPaused();
}

void ClockScreen::togglePause() {
  pause(start_millis != PAUSED);
}




void ClockScreen::sayTime(bool big) {
  sayTime(orient,big);
}

void ClockScreen::sayTime(uint8_t r, bool big) {
  uint16_t xx, yy;
  char msg[9];
  tft.setRotation(r);
  Menu::screenOffset(r,xx,yy);
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

void ClockScreen::sayPaused() {
  uint16_t xx, yy;
  Menu::screenOffset(orient,xx,yy);
  tft.setRotation(orient);
  if (start_millis == PAUSED) {
    tft.setTextSize(3);
    tft.setTextColor(SCREEN_FG, SCREEN_BG);
    tft.setCursor(xx + 70, yy + 160);
    tft.print(F("Paused"));
  } else {
    tft.fillRect(xx + 40, yy + 140, 160, 60, SCREEN_BG);
  }
}


void ClockScreen::activate(uint8_t r) {
  uint16_t xx;
  uint16_t yy;

  if (r != orient) {
    // Say the total time for the current player
    uint8_t rold = orient;
    Menu::screenOffset(rold, xx, yy);
    tft.setRotation(rold);
    sayTime(SMALL);

    // Clear the screen.  We flip to the other side, so that the slow
    // drawing gives us a nice "roll up" effect.
    rold = (rold + 2) % 4;
    tft.setRotation(rold);
    Menu::screenOffset(rold, xx,yy);
    tft.fillRect(xx + 40, yy + 40, 160, 160, SCREEN_BG);
    orient = r;
  }

  if (r == DIR_UNKNOWN) {
    pause(true);
  } else {
    // Now switch to the new side.
    tft.setRotation(r);
    Menu::screenOffset(r, xx, yy);
    gradRect(xx + 40, yy, 160, 140, SCREEN_BG, msgBG[r]);
    sayTime(BIG);
    start_millis = millis();
  }

  sayPaused();
}








