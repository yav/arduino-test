#ifndef __CLOCK_SCREEN_H_
#define __CLOCK_SCREEN_H_

#include "Screen.h"
#include "Menu.h"

class ClockScreen : public Screen, private Menu {

  unsigned long start_millis;
  unsigned long ptime[4];
  uint8_t orient;

public:
  ClockScreen();
  void setup()                        override;
  void update()                       override;
  void rotated(uint8_t d)             override;
  void onDown(uint16_t x, uint16_t y) override;
  void onUp() override;

private:
  MenuLabels menuLabels()             override;
  void menuAction(uint8_t i)          override;
  void pause ();
  void sayTime(bool big);
  void sayTime(uint8_t r, bool big);
  void sayPaused();
  void activate(uint8_t r);
};

#endif




