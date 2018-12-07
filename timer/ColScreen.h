#ifndef __COL_SCREEN_H__
#define __COL_SCREEN_H__

#include "Screen.h"
#include "Menu.h"

class ColScreen : public Screen, private Menu {

  uint8_t owner[10]; // who owns a color
  uint8_t slots[4]; // what is selected for each slot
  int next_p;       // which slot to fill next

public:
  ColScreen();
  void setup(uint8_t d)               override;
  void onDown(uint16_t x, uint16_t y) override;
  void onUp()                         override;
  void rotated(uint8_t d)             override;

private:
  MenuLabels menuLabels()             override;
  void menuAction(uint8_t i)          override;
  bool buttonUsed(uint8_t i)          override;

  void nextSlot();
  void fillSlot(uint8_t col);
  void freeSlot(uint8_t col);
};

#endif
