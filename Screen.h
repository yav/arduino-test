#ifndef __YSCREEN_H_
#define __YSCREEN_H_

#include <inttypes.h>


// A specific "screen" of an application
class Screen {
  public:

    // Called when we want to display the screen
    virtual void setup(void) {}

    // Called in a loop to update the screen
    virtual void update(void) {}

    // Called to handle touch events
    virtual void onDown(uint16_t x, uint16_t y) {}
    virtual void onUp(void) {}

    // Called when rotated (input 0--3)
    virtual void rotated(uint8_t d) {}
};




#endif
