BOARD_TAG= uno
ARDUINO_DIR=../arduino-1.8.7
ARDMK_DIR=../Arduino-Makefile
AVR_TOOLS_DIR=../arduino-1.8.7/hardware/tools/avr
ARDUINO_LIBS=Adafruit_GFX_Library \
             Adafruit_ILI9341 \
             SPI \
             Adafruit_STMPE610 \
             Wire \
             Adafruit_Unified_Sensor \
             Adafruit_LIS3DH \
             SD

include $(ARDMK_DIR)/Arduino.mk



