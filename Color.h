#ifndef __COLOR_H_
#define __COLOR_H_


static inline
uint16_t COL565(int r, int g, int b) {
  return ((r&0x1F) << 11) | ((g&0x3F) << 5) | (b&0x1F);
}

static inline
uint16_t COL888(int r, int g, int b) {
  return COL565(r >> 3, g >> 2, b >> 3);
}

static inline
int d4(int x) {
  int y = x & 0xF;
  return (y << 4) | y;
}

static inline
uint16_t COL444(int r, int g, int b) {
  return COL888(d4(r), d4(g), d4(b));
}

static inline
uint16_t COL444(int rgb) {
  return COL444(rgb>>8,rgb>>4,rgb);
}

#endif
