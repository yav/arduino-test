#include <Adafruit_CircuitPlayground.h>

uint8_t t;

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
}


void loop() {
  CircuitPlayground.setPixelColor (t,        0,  0, 0);
  CircuitPlayground.setPixelColor ((t+1)%10, 20, 0, 0);
  CircuitPlayground.setPixelColor ((t+2)%10, 200, 0, 0);
  CircuitPlayground.setPixelColor ((t+3)%10, 20,    0, 0);
  t = (t + 1) % 10;
  delay(500);
}
