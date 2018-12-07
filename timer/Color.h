#ifndef __COLOR_H_
#define __COLOR_H_

#define COL565_R(c) (((c) >> 11) & 0x1F)
#define COL565_G(c) (((c) >>  5) & 0x3F)
#define COL565_B(c) (((c) >>  0) & 0x1F)

#define COL565(r,g,b) \
  ((((uint16_t)(r)&0x1F) << 11) | (((uint16_t)(g)&0x3F) << 5) | ((uint16_t)(b)&0x1F))

#define COL888(r,g,b) (COL565(r >> 3, g >> 2, b >> 3))

#define d4(x) ((((x)&0xF) << 4) | ((x)&0xF))

#define COL444(r,g,b) (COL888(d4(r),d4(g),d4(b)))

#define COL(rgb) (COL444(rgb>>8,rgb>>4,rgb))


#endif
