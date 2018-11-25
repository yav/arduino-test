#ifndef __YSCREEN_H_
#define __YSCREEN_H_

// A specific "screen" of an application
class Screen {
  public:

    // Called when we want to display the screen
    virtual void setup(void) = 0;

    // Called in a loop to update the screen
    virtual void update(void) = 0;

    // Called to handle touch events
    virtual void onDown(uint16_t x, uint16_t y) = 0;
    virtual void onUp(void) = 0;

    // Called when rotated (input 0--3)
    virtual void rotated(uint8_t d) = 0;
};




#endif
