// ====================================================================
// RD_Comun.h - shared components used by all faces
// Do not include directly; included by RelojDinamico.h.
// ====================================================================
#ifndef RD_COMUN_H
#define RD_COMUN_H

#ifndef RELOJ_DINAMICO_H
#error "Include RelojDinamico.h instead of this file"
#endif

// Components shared by several faces: constants, sprites, clouds, the 5x7 digit font,
// and Mario sprite (RDM_*), rdm_digitos, rdm_cieloPorHora, rdi_hash, and
// rectangle-based sprite utilities (RDRect, rdRectColor, rdBlanco, rdDigito2).

#define RDM_TICK_MS   40       // one logic "tick" = 25 fps
#define RDM_SUELO     29       // first ground row
#define RDM_Y0        16       // top row of the idle sprite (feet at y=28)
#define RDM_BLK_Y     1        // top row of the blocks
#define RDM_XMIN      3
#define RDM_XMAX      114      // the sprite is 11 pixels wide
static const int16_t RDM_BLK_X[4] = { 35, 49, 69, 83 };   // 12x12 blocks: H1 H2 : M1 M2
static const int8_t  RDM_SALTO[]  = { 1, 2, 3, 4, 4, 3, 2, 1, 0 };   // height per tick
#define RDM_SALTO_N   ((int)sizeof(RDM_SALTO))
#define RDM_SALTO_GOLPE 3      // jump tick when the block is hit (the first 4)

// 11x13 sprites (facing right). '.' = transparent
static const char RDM_STAND[13][12] PROGMEM = {
  "...RRRRR...", "..RRRRRRRRR", "..BBBSSKS..", ".BSBSSSKSSS", ".BSBBSSSKSS",
  ".BBSSSSKKK.", "...SSSSSS..", "..RRUUURR..", ".RRRUUURRR.", ".SSUUYUUSS.",
  "..UUUUUUU..", "..UUU.UUU..", ".BBBB.BBBB."
};
static const char RDM_RUN1[13][12] PROGMEM = {
  "...RRRRR...", "..RRRRRRRRR", "..BBBSSKS..", ".BSBSSSKSSS", ".BSBBSSSKSS",
  ".BBSSSSKKK.", "...SSSSSS..", "..RRUUURR..", ".RRRUUURRR.", "..SUUYUUS..",
  "..UUUUUUU..", ".UUU...UUU.", "BBBB...BBBB"
};
static const char RDM_JUMP[13][12] PROGMEM = {
  "...RRRRR...", "..RRRRRRRRR", "..BBBSSKS..", ".BSBSSSKSSS", ".BSBBSSSKSS",
  ".BBSSSSKKK.", "...SSSSSS..", ".SRRUUURRS.", ".SRRUUURRS.", "..UUUYUUU..",
  "..UUUUUUU..", ".UUUU.UUUU.", ".BBB...BBB."
};
static const char RDM_NUBE[5][15] PROGMEM = {
  "....WWWW......", "..WWWWWWWW.WW.", ".WWWWWWWWWWWWW", "WWWWWWWWWWWWWW", ".GGGGGGGGGGGG."
};
static const char RDM_ARBUSTO[5][17] PROGMEM = {
  "....LLLL........", "..LLLLLLLL.LLL..", ".LLLLLLLLLLLLLL.", "LLLLLLLLLLLLLLLL", "LLDLLLLDLLLLDLLL"
};
// 5x7 digits (bit4 = left column)
static const uint8_t RDM_FUENTE[10][7] PROGMEM = {
  {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},
  {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}, {0x1E,0x01,0x01,0x0E,0x01,0x01,0x1E},
  {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},
  {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}, {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},
  {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}
};

static const int8_t RDM_NUBE_Y[2] = { 2, 9 };

static uint8_t rdm_cieloPorHora(int h) { return (h >= 7 && h < 19) ? 0 : (h >= 19 && h < 21) ? 1 : 2; }

static void rdm_digitos(int h, int m, int8_t* o) { o[0] = h / 10; o[1] = h % 10; o[2] = m / 10; o[3] = m % 10; }

static uint16_t rdm_colorSprite(char ch) {
  switch (ch) {
    case 'R': return RD_C(216, 40, 0);
    case 'B': return RD_C(120, 64, 0);
    case 'S': return RD_C(252, 184, 120);
    case 'K': return RD_C(24, 16, 16);
    case 'U': return RD_C(40, 80, 216);
    case 'Y': return RD_C(252, 216, 0);
  }
  return 0;
}

static uint8_t rdi_hash(int x, int y, int t) {
  unsigned v = (unsigned)x * 7919u + (unsigned)y * 104729u + (unsigned)t * 1299709u;
  v ^= v >> 13; v *= 0x5bd1e995u; v ^= v >> 15;
  return (uint8_t)(v & 0xFF);
}

#ifndef RD_UTILIDADES
#define RD_UTILIDADES
// Rectangle-based sprite: x, y, width, height, color index (1..15). The last rectangle is drawn on top.
struct RDRect { int8_t x, y, w, h; uint8_t c; };
static uint8_t rdRectColor(const RDRect* r, int n, int lx, int ly) {
  for (int i = n - 1; i >= 0; i--) {
    int x = (int8_t)pgm_read_byte(&r[i].x), y = (int8_t)pgm_read_byte(&r[i].y);
    int w = pgm_read_byte(&r[i].w), h = pgm_read_byte(&r[i].h);
    if ((unsigned)(lx - x) < (unsigned)w && (unsigned)(ly - y) < (unsigned)h) return pgm_read_byte(&r[i].c);
  }
  return 0;
}
// Blend an RGB565 color with white: k = 0..8 (8 = white)
static uint16_t rdBlanco(uint16_t c, int k) {
  int r = (c >> 11) & 31, g = (c >> 5) & 63, b = c & 31;
  r += ((31 - r) * k) >> 3; g += ((63 - g) * k) >> 3; b += ((31 - b) * k) >> 3;
  return (uint16_t)((r << 11) | (g << 5) | b);
}
// Does digit d (5x7 font scaled x2 = 10x14) have a pixel at (lx, ly)?
static inline bool rdDigito2(int d, int lx, int ly) {
  if ((unsigned)lx >= 10 || (unsigned)ly >= 14) return false;
  return (pgm_read_byte(&RDM_FUENTE[d][ly >> 1]) & (0x10 >> (lx >> 1))) != 0;
}
#endif

#endif // RD_COMUN_H
