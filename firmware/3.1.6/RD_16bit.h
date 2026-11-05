// ====================================================================
// RD_16bit.h - 16-bit faces
// Do not include directly; included by RelojDinamico.h.
// ====================================================================
#ifndef RD_16BIT_H
#define RD_16BIT_H

#ifndef RELOJ_DINAMICO_H
#error "Include RelojDinamico.h instead of this file"
#endif

// ────────────────────────────────────────────────────────────────────
//  "Sonic"
// ────────────────────────────────────────────────────────────────────
#define RDS_SUELO     28       // first ground row
#define RDS_PIES      27       // row of feet on the ground
#define RDS_MON_Y     16       // top row of monitors (12x12, through y=27)
#define RDS_PIES_MON  15       // row of feet while standing on a monitor
static const int16_t RDS_MON_X[4] = { 35, 49, 69, 83 };

// 16x18 sprites (facing right). '.' = transparent
// B blue, b dark blue, L highlight, P light skin, p dark skin, W white, K black, G green, R red, r dark red, Y gold
static const char RDS_STAND[18][17] PROGMEM = {
  "........BB......", ".BBBB..BLLBB....", "..BBBBBLBWWWB...",
  "....BBBBWWWWWB..", "...bBBBBWWGWKB..", ".bbbBBBBWWGKKB..",
  "..bbBBBBWWWKKPP.", "....BBBBBWWWPPPK", "...bBBBBBPPPPPPK",
  "..bbbbbBBBPpppP.", ".....bbBPPPB....", "...WWBBBPPPBBWW.",
  "...WWBBBPPPBBWW.", "...WW.BBBPPBWW..", "......BB..BB....",
  "....rrWWrRRWWR..", "...rrrrYRRRRYRR.", "....rrrrrRRRRR.."
};
static const char RDS_TAP[18][17] PROGMEM = {
  "........BB......", ".BBBB..BLLBB....", "..BBBBBLBWWWB...",
  "....BBBBWWWWWB..", "...bBBBBWWGWKB..", ".bbbBBBBWWGKKB..",
  "..bbBBBBWWWKKPP.", "....BBBBBWWWPPPK", "...bBBBBBPPPPPPK",
  "..bbbbbBBBPpppP.", ".....bbBPPPB....", "...WWBBBPPPBBWW.",
  "...WWBBBPPPBBWW.", "...WW.BBBPPBWW..", "......BB.RRWWR..",
  "....rrWWRRRRYRR.", "...rrrrYrRRRRR..", "....rrrrr......."
};
static const char RDS_RUN1[18][17] PROGMEM = {
  "........BB......", ".BBBB..BLLBB....", "..BBBBBLBWWWB...",
  "....BBBBWWWWWB..", "...bBBBBWWGWKB..", ".bbbBBBBWWGKKB..",
  "..bbBBBBWWWKKPP.", "....BBBBBWWWPPPK", "...bBBBBBPPPPPPK",
  "..bbbbbBBBPpppP.", "...WWWbBPPPBBWW.", "...WWBBBPPPBBW..",
  "...WWBBBPPP.....", "....BBBBBPP.BB..", "....BB......BB..",
  "....BB.....RRWWR", ".rrWWr....RRRRYR", "rrrrYrr....RRRRR"
};
static const char RDS_RUN2[18][17] PROGMEM = {
  "........BB......", ".BBBB..BLLBB....", "..BBBBBLBWWWB...",
  "....BBBBWWWWWB..", "...bBBBBWWGWKB..", ".bbbBBBBWWGKKB..",
  "..bbBBBBWWWKKPP.", "....BBBBBWWWPPPK", "...bBBBBBPPPPPPK",
  "..bbbbbBBBPpppP.", "...WWWbBPPPBBWW.", "...WWBBBPPPBBW..",
  "...WWBBBPPP.....", "......BBBPP.....", ".......BBBB.....",
  ".....rrWRRWWR...", "....rrrRRRRYRR..", ".....rrrRRRRR..."
};
static const char RDS_ANILLO[6][7] PROGMEM = {
  ".gGGg.", "gG..Gg", "G....G", "G....G", "gG..Gg", ".gGGg."
};
static const char RDS_PALMERA[22][14] PROGMEM = {
  "..LL.LLL.LL..", ".LLLLLLLLLLL.", "LLLLLLTTLLLLL", "L..LLLTTLL..L", "L....LTT....L",
  "......TT.....", "......tt.....", "......TT.....", "......tt.....", "......TT.....",
  "......tt.....", "......TT.....", "......tt.....", "......TT.....", "......tt.....",
  "......TT.....", "......tt.....", "......TT.....", "......tt.....", "......TT.....",
  "......tt.....", "......TT....."
};
static const uint8_t RDS_COLINA[16] PROGMEM = { 0,1,2,3,4,4,3,2,1,0,1,2,3,3,2,1 };

struct RDSEstado {
  int8_t   d[4], obj[4];
  uint8_t  pend;
  uint8_t  modo;        // 0 idle (keeps the beat), 1 walks, 2 charges, 3 flies, 4 hits
  int8_t   dir;
  int16_t  fx, fy;      // feet center
  uint8_t  anim;        // 0 still, 1 foot raised, 2-3 stride
  uint8_t  cielo;
  uint8_t  flash[4];
  int8_t   bloque;      // target monitor
  int16_t  x0, y0, x1, y1;
  uint8_t  t, N, H;
  bool     aSuelo;
  uint8_t  espera;
  uint8_t  zona;        // 0 left, 1 right
  int16_t  destino;
  uint8_t  anillo;      // ticks remaining for the ring (0 = none)
  int16_t  anilloX;
  int16_t  nube[2];
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDSEstado rds;

static void rds_marcarSonic()  { rdMarcar(rds.fx - 12, rds.fy - 19, 25, 21); }
static void rds_marcarMon(int i) { rdMarcar(RDS_MON_X[i], RDS_MON_Y, 12, 12); }
static void rds_marcarAnillo() { rdMarcar(rds.anilloX - 3, 0, 6, 16); }
static void rds_marcarNube(int i) { rdMarcar(rds.nube[i], RDM_NUBE_Y[i], 14, 5); }

static void rds_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rds.d);
  for (int i = 0; i < 4; i++) { rds.obj[i] = rds.d[i]; rds.flash[i] = 0; }
  rds.pend = 0; rds.modo = 0; rds.dir = 1; rds.fx = 18; rds.fy = RDS_PIES;
  rds.anim = 0; rds.bloque = -1; rds.espera = 40 + (m % 30); rds.zona = 0;
  rds.anillo = 0; rds.n = 0; rds.aSuelo = false;
  rds.nube[0] = 8; rds.nube[1] = 96;
  rds.dp = (s % 2 == 0);
  rds.cielo = rdm_cieloPorHora(h);
  rds.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rds_elegir() {
  int mejor = -1, dmin = 9999;
  for (int i = 0; i < 4; i++) {
    if (!(rds.pend & (1 << i))) continue;
    int dist = abs((RDS_MON_X[i] + 6) - rds.fx);
    if (dist < dmin) { dmin = dist; mejor = i; }
  }
  rds.bloque = mejor;
}

static void rds_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rds.obj[i] = nd[i]; if (nd[i] != rds.d[i]) mask |= (1 << i); }
  uint8_t c = rdm_cieloPorHora(h);
  if (c != rds.cielo) { rds.cielo = c; rdMarcar(0, 0, RD_W, RD_H); }
  rds.pend = mask;
  if (mask && rds.modo <= 1) {
    rds_elegir();
    rds.modo = 2; rds.espera = 10; rds.anim = 0;
    rds.dir = ((RDS_MON_X[rds.bloque] + 6) >= rds.fx) ? 1 : -1;
  }
}

static void rds_segundo(int s) {
  rds.dp = (s % 2 == 0);
  rdMarcar(64, 18, 2, 8);
}

// Start an arcing jump toward (x1, y1); H = extra arc height
static void rds_volar(int x1, int y1, int H, bool suelo) {
  rds.x0 = rds.fx; rds.y0 = rds.fy; rds.x1 = x1; rds.y1 = y1;
  int dist = abs(x1 - rds.fx);
  int N = 6 + dist / 4; if (N > 26) N = 26;
  rds.N = N; rds.t = 0; rds.H = H; rds.aSuelo = suelo;
  if (x1 != rds.fx) rds.dir = (x1 > rds.fx) ? 1 : -1;
  rds.modo = 3;
}

static void rds_siguiente() {              // after a hit: another monitor or return to the ground
  if (rds.pend) { rds_elegir(); rds_volar(RDS_MON_X[rds.bloque] + 6, RDS_PIES_MON, 5, false); }
  else          { rds.bloque = -1; rds_volar(rds.fx < 64 ? 20 : 108, RDS_PIES, 4, true); }
}

static void rds_tick() {
  rds_marcarSonic();
  rds.n++;

  for (int i = 0; i < 4; i++) if (rds.flash[i]) { rds.flash[i]--; rds_marcarMon(i); }
  if (rds.anillo) { rds_marcarAnillo(); rds.anillo--; rds_marcarAnillo(); }
  if ((rds.n % 12) == 0) { rds_marcarNube(0); if (--rds.nube[0] < -14) rds.nube[0] = RD_W; rds_marcarNube(0); }
  if ((rds.n % 20) == 0) { rds_marcarNube(1); if (--rds.nube[1] < -14) rds.nube[1] = RD_W; rds_marcarNube(1); }

  switch (rds.modo) {
    case 0:                                              // taps a foot to keep the beat
      rds.anim = ((rds.n / 6) & 1) ? 1 : 0;
      if (rds.espera) rds.espera--;
      if (!rds.espera) {
        int zmin = rds.zona ? 102 : 10, zmax = rds.zona ? 118 : 26;
        rds.destino = ((rds.fx - zmin) < (zmax - rds.fx)) ? zmax : zmin;
        rds.dir = (rds.destino > rds.fx) ? 1 : -1;
        rds.modo = 1;
      }
      break;
    case 1:                                              // jogs to the other end of its area
      rds.fx += rds.dir;
      rds.anim = 2 + ((rds.n / 2) & 1);
      if (rds.fx == rds.destino) {
        rds.modo = 0; rds.anim = 0;
        rds.dir = rds.zona ? -1 : 1;                     // faces the monitors
        rds.espera = 50 + (rds.n % 40);
      }
      break;
    case 2:                                              // spin dash: charges in place
      if (rds.espera) rds.espera--;
      if (!rds.espera) rds_volar(RDS_MON_X[rds.bloque] + 6, RDS_PIES_MON, 5, false);
      break;
    case 3: {                                            // arcing flight
      rds.t++;
      int p = (rds.t * 256) / rds.N;
      rds.fx = rds.x0 + ((rds.x1 - rds.x0) * p) / 256;
      rds.fy = rds.y0 + ((rds.y1 - rds.y0) * p) / 256 - ((4 * rds.H * p * (256 - p)) >> 16);
      if (rds.t >= rds.N) {
        rds.fx = rds.x1; rds.fy = rds.y1;
        if (rds.aSuelo) {
          rds.zona = (rds.fx < 64) ? 0 : 1;
          rds.dir = rds.zona ? -1 : 1;
          rds.modo = 0; rds.anim = 0; rds.espera = 40 + (rds.n % 30);
        } else {                                         // hit!
          int b = rds.bloque;
          rds.d[b] = rds.obj[b];
          rds.flash[b] = 3;
          rds.pend &= ~(1 << b);
          rds.anillo = 7; rds.anilloX = RDS_MON_X[b] + 6;
          rds_marcarMon(b); rds_marcarAnillo();
          rds.modo = 4; rds.espera = 3;
        }
      }
    } break;
    case 4:                                              // brief pause on the monitor
      if (rds.espera) rds.espera--;
      if (!rds.espera) rds_siguiente();
      break;
  }
  rds_marcarSonic();
}

static void rds_paso(uint32_t ms) {
  if (!rds.ultOk) { rds.ult = ms; rds.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rds.ult) >= RDM_TICK_MS && n < 3) { rds.ult += RDM_TICK_MS; rds_tick(); n++; }
  if (n == 3) rds.ult = ms;
}

static uint16_t rds_colorSprite(char ch) {
  switch (ch) {
    case 'B': return RD_C(24, 72, 216);
    case 'P': return RD_C(250, 200, 150);
    case 'W': return RD_C(252, 252, 252);
    case 'K': return RD_C(16, 16, 24);
    case 'b': return RD_C(14, 44, 160);
    case 'L': return RD_C(90, 140, 250);
    case 'p': return RD_C(215, 160, 110);
    case 'G': return RD_C(40, 170, 80);
    case 'r': return RD_C(160, 10, 10);
    case 'Y': return RD_C(250, 210, 40);
    case 'R': return RD_C(230, 24, 24);
  }
  return 0;
}

static uint16_t rds_pixel(int x, int y) {
  // 1) Ring launched from the monitor
  if (rds.anillo) {
    int lx = x - (rds.anilloX - 3), ly = y - (2 + rds.anillo);
    if ((unsigned)lx < 6 && (unsigned)ly < 6) {
      char ch = (char)pgm_read_byte(&RDS_ANILLO[ly][lx]);
      if (ch == 'G') return RD_C(252, 200, 0);
      if (ch == 'g') return RD_C(200, 140, 0);
    }
  }
  // 2) Hedgehog: standing (modes 0-1) or curled up (2-4)
  if (rds.modo <= 1) {
    int lx = x - (rds.fx - 8), ly = y - (rds.fy - 17);
    if ((unsigned)lx < 16 && (unsigned)ly < 18) {
      const char (*spr)[17] = (rds.anim == 0) ? RDS_STAND : (rds.anim == 1) ? RDS_TAP : (rds.anim == 2) ? RDS_RUN1 : RDS_RUN2;
      char ch = (char)pgm_read_byte(&spr[ly][(rds.dir < 0) ? 15 - lx : lx]);
      if (ch != '.') return rds_colorSprite(ch);
    }
  } else {
    int lx = x - (rds.fx - 5), ly = y - (rds.fy - 9);
    if ((unsigned)lx < 10 && (unsigned)ly < 10) {
      int ax = 2 * lx - 9, ay = 2 * ly - 9;
      int r2 = ax * ax + ay * ay;
      if (r2 <= 81) {
        if (r2 > 49) return RD_C(12, 40, 150);                       // outline
        static const int8_t NX[4] = { 1, 1, 0, 1 }, NY[4] = { 0, 1, 1, -1 };
        int f = rds.n & 3;
        int v = ax * NX[f] + ay * NY[f]; if (v < 0) v = -v;
        return (v <= 2) ? RD_C(120, 170, 252) : RD_C(24, 72, 216);
      }
    }
    if (rds.modo == 2) {                                              // spin dash dust
      for (int k = 0; k < 3; k++) {
        int px = rds.fx - rds.dir * (7 + k * 2), py = RDS_PIES - ((int)(rds.n + k) % 3);
        if (x == px && y == py) return RD_C(230, 220, 200);
      }
    }
  }
  // 3) Checkerboard ground
  if (y >= RDS_SUELO) {
    if (y == RDS_SUELO) return ((x & 3) == 0) ? RD_C(120, 252, 80) : RD_C(40, 200, 40);
    bool a = (((x >> 2) & 1) ^ (y == RDS_SUELO + 3 ? 1 : 0)) != 0;
    return a ? RD_C(206, 126, 48) : RD_C(150, 80, 20);
  }
  // 4) Monitors with digits
  if (y >= RDS_MON_Y) {
    for (int i = 0; i < 4; i++) {
      int lx = x - RDS_MON_X[i];
      if ((unsigned)lx >= 12) continue;
      int ly = y - RDS_MON_Y;
      if (lx == 0 || lx == 11 || ly == 0 || ly == 11) return RD_C(190, 190, 205);
      if (lx == 1 || lx == 10 || ly == 1 || ly == 10) return RD_C(70, 70, 90);
      bool fl = rds.flash[i] != 0;
      int dx = lx - 3, dy = ly - 2;
      if ((unsigned)dx < 5 && (unsigned)dy < 7 &&
          (pgm_read_byte(&RDM_FUENTE[rds.d[i]][dy]) & (0x10 >> dx)))
        return fl ? RD_C(8, 16, 64) : RD_C(200, 252, 252);
      return fl ? RD_C(252, 252, 252) : RD_C(8, 16, 64);
    }
    // colon between the monitors
    if (rds.dp && (unsigned)(x - 64) < 2 && ((unsigned)(y - 18) < 2 || (unsigned)(y - 24) < 2))
      return RD_C(252, 200, 0);
  }
  // 5) Palm tree
  {
    int lx = x - 104, ly = y - 6;
    if ((unsigned)lx < 13 && (unsigned)ly < 22) {
      char ch = (char)pgm_read_byte(&RDS_PALMERA[ly][lx]);
      if (ch == 'L') return RD_C(30, 170, 50);
      if (ch == 'T') return RD_C(150, 90, 30);
      if (ch == 't') return RD_C(110, 64, 20);
    }
  }
  // 6) Distant hills
  {
    int top = 20 - (int)pgm_read_byte(&RDS_COLINA[(x >> 2) & 15]);
    if (y >= top) return RD_C(16, 120, 80);
  }
  // 7) Sun / moon and stars
  {
    int cx = (rds.cielo == 2) ? 12 : 120, cy = (rds.cielo == 0) ? 5 : (rds.cielo == 1) ? 12 : 6;
    int dx = x - cx, dy = y - cy;
    if (dx * dx + dy * dy <= 9)
      return rds.cielo == 0 ? RD_C(252, 232, 120) : rds.cielo == 1 ? RD_C(252, 150, 60) : RD_C(225, 225, 250);
    if (rds.cielo == 2 && y < 14 && ((x * 7 + y * 13) % 61) == 0) return RD_C(200, 200, 255);
  }
  // 8) Clouds
  for (int i = 0; i < 2; i++) {
    int lx = x - rds.nube[i], ly = y - RDM_NUBE_Y[i];
    if ((unsigned)lx < 14 && (unsigned)ly < 5) {
      char ch = (char)pgm_read_byte(&RDM_NUBE[ly][lx]);
      if (ch == 'W') return rds.cielo == 0 ? RD_C(252, 252, 252) : rds.cielo == 1 ? RD_C(161, 161, 181) : RD_C(70, 70, 110);
      if (ch == 'G') return rds.cielo == 0 ? RD_C(188, 208, 252) : rds.cielo == 1 ? RD_C(118, 128, 168) : RD_C(48, 48, 84);
    }
  }
  // 9) Sky
  return rds.cielo == 0 ? RD_C(40, 152, 252) : rds.cielo == 1 ? RD_C(23, 79, 146) : RD_C(6, 6, 40);
}

// ────────────────────────────────────────────────────────────────────
// "MegaManX"
// ────────────────────────────────────────────────────────────────────
#define RDX_DIG_Y      1       // top row of blocks (14 pixels high -> through y=14)
#define RDX_Y0         16      // top row of X (13 pixels high -> feet at y=28)
#define RDX_MUZZLE_Y   24      // cannon height (RDX_Y0 + 8)
static const int16_t RDX_DIG_X[4] = { 35, 48, 69, 82 };     // digits are 10 pixels wide

// 11x13 sprites (facing right). B armor, L highlight, D shadow, S face, K eye, R gem, G cannon muzzle
static const char RDX_STAND[13][12] PROGMEM = {
  "...BBBBB...", "..BBLLBBB..", ".BBBRBBBBB.", ".BBSSSSSDB.", ".BDSSKSSSD.",
  "..DSSSSSD..", ".BBBLLBBBB.", ".BLBBBBBDDD", ".BBBBBBBDGD", "..BBBBBBB..",
  "..BBB.BBB..", "..BBB.BBB..", ".DDDD.DDDD."
};
static const char RDX_RUN[13][12] PROGMEM = {
  "...BBBBB...", "..BBLLBBB..", ".BBBRBBBBB.", ".BBSSSSSDB.", ".BDSSKSSSD.",
  "..DSSSSSD..", ".BBBLLBBBB.", ".BLBBBBBDDD", ".BBBBBBBDGD", "..BBBBBBB..",
  "..BBB..BBB.", ".BBB....BBB", ".DDD....DDD"
};

struct RDXEstado {
  int8_t   d[4], obj[4];
  uint8_t  ph[4], rt[4];    // 0 normal, 1 broken (hidden), 2 rebuilding (rt = tick)
  uint8_t  pend;
  uint8_t  modo;            // 0 runs in place, 1 runs to target, 2 charges, 3 fires, 4 pauses
  int8_t   bloque, dir;
  int8_t   bd, bod;         // digit being destroyed and its previous value
  uint8_t  anim, tm, bt, sc;
  int16_t  x, tx, sx, sy;
  int16_t  qx[35], qy[35];  // position of each flying block (-999 = none)
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDXEstado rdx;

static void rdx_marcarX()     { rdMarcar(rdx.x - 2, 14, 22, 18); }                 // includes the halo
static void rdx_marcarTiro()  { rdMarcar(rdx.sx - 6, rdx.sy - 6, 13, 13); }
static void rdx_marcarRafaga(){ rdMarcar(RDX_DIG_X[rdx.bd] - 30, 0, 70, 32); }     // flight area
static void rdx_marcarDigito(int i) { rdMarcar(RDX_DIG_X[i], 0, 10, 16); }

static void rdx_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdx.d);
  for (int i = 0; i < 4; i++) { rdx.obj[i] = rdx.d[i]; rdx.ph[i] = 0; rdx.rt[i] = 0; }
  rdx.pend = 0; rdx.modo = 0; rdx.bloque = -1; rdx.dir = 1;
  rdx.bd = 0; rdx.bod = 0; rdx.anim = 0; rdx.tm = 0; rdx.bt = 0; rdx.sc = 0;
  rdx.x = 4 + (m % 20); rdx.tx = rdx.x; rdx.sx = 0; rdx.sy = 0;
  for (int k = 0; k < 35; k++) { rdx.qx[k] = -999; rdx.qy[k] = 0; }
  rdx.dp = (s % 2 == 0); rdx.n = 0; rdx.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

// Nearest target: X positions himself so the diagonal shot reaches the bottom of the block
static void rdx_elegir() {
  int mejor = -1, dmin = 9999;
  for (int i = 0; i < 4; i++) {
    if (!(rdx.pend & (1 << i))) continue;
    int dist = abs((RDX_DIG_X[i] - 17) - rdx.x);
    if (dist < dmin) { dmin = dist; mejor = i; }
  }
  rdx.bloque = mejor;
  if (mejor >= 0) rdx.tx = RDX_DIG_X[mejor] - 17;
}

static void rdx_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rdx.obj[i] = nd[i]; if (nd[i] != rdx.d[i]) mask |= (1 << i); }
  rdx.pend = mask;
  if (mask && rdx.modo == 0) { rdx_elegir(); rdx.modo = 1; }
}

static void rdx_segundo(int s) { rdx.dp = (s % 2 == 0); rdMarcar(62, 3, 2, 10); }

// Position of each block in the broken digit on tick rdx.bt: fans out from the impact and falls
static void rdx_escombros() {
  int i = rdx.bd, t = rdx.bt;
  int ix = RDX_DIG_X[i] + 5, iy = 14;
  for (int k = 0; k < 35; k++) {
    int cx = k % 5, cy = k / 5;
    if (!(pgm_read_byte(&RDM_FUENTE[rdx.bod][cy]) & (0x10 >> cx))) { rdx.qx[k] = -999; continue; }
    int sx = RDX_DIG_X[i] + cx * 2, sy = RDX_DIG_Y + cy * 2;
    uint8_t hh = rdi_hash(i * 35 + k, 7, 3);
    int vx = (sx + 1 - ix) * 2 + (int)(hh & 7) - 3;
    int vy = (sy + 1 - iy) + (int)((hh >> 3) & 7) - 3 - 5;
    rdx.qx[k] = sx + (vx * t) / 8;
    rdx.qy[k] = sy + (vy * t) / 8 + (t * t) / 5;
  }
}

static void rdx_tick() {
  rdx_marcarX();
  rdx.n++;

  // Breakup: blocks fly out and rebuilding starts halfway through
  if (rdx.bt) {
    rdx_marcarRafaga();
    rdx.bt++;
    if (rdx.bt == 10) { int i = rdx.bd; rdx.d[i] = rdx.obj[i]; rdx.ph[i] = 2; rdx.rt[i] = 1; }
    if (rdx.bt > 22) { rdx.bt = 0; for (int k = 0; k < 35; k++) rdx.qx[k] = -999; }
    else rdx_escombros();
    rdx_marcarRafaga();
  }
  for (int i = 0; i < 4; i++) {
    if (rdx.ph[i] != 2) continue;
    rdx.rt[i]++; rdx_marcarDigito(i);
    if (rdx.rt[i] >= 18) { rdx.ph[i] = 0; rdx.rt[i] = 0; }
  }

  // Highway stripes move only while X is running
  if (rdx.modo <= 1 && (rdx.n % 3) == 0) { rdx.sc = (rdx.sc + 1) & 15; rdMarcar(0, 24, RD_W, 8); }

  switch (rdx.modo) {
    case 0:                                         // runs in place
      if ((rdx.n % 3) == 0) rdx.anim ^= 1;
      break;

    case 1:                                         // runs to firing position
      if (rdx.x < rdx.tx)      { rdx.x += (rdx.tx - rdx.x >= 2) ? 2 : 1; rdx.dir = 1; }
      else if (rdx.x > rdx.tx) { rdx.x -= (rdx.x - rdx.tx >= 2) ? 2 : 1; rdx.dir = -1; }
      if ((rdx.n % 2) == 0) rdx.anim ^= 1;
      if (rdx.x == rdx.tx) { rdx.dir = 1; rdx.modo = 2; rdx.tm = 0; }
      break;

    case 2:                                         // charges the cannon
      rdx.tm++;
      if (rdx.tm >= 14) {
        rdx.modo = 3; rdx.tm = 0;
        rdx.sx = rdx.x + 12; rdx.sy = RDX_MUZZLE_Y;
        rdx_marcarTiro();
      }
      break;

    case 3:                                         // the plasma ball travels diagonally upward
      rdx_marcarTiro();
      rdx.sx += 2; rdx.sy -= 2; rdx.tm++;
      rdx_marcarTiro();
      if (rdx.sy <= 14) {                           // impact at the bottom of the block
        int b = rdx.bloque;
        rdx.bd = b; rdx.bod = rdx.d[b];
        rdx.ph[b] = 1; rdx.bt = 1;
        rdx.pend &= ~(1 << b);
        rdx_escombros(); rdx_marcarRafaga();
        rdx.modo = 4; rdx.tm = 0;
      }
      break;

    case 4:                                         // pause, then next target
      rdx.tm++;
      if (rdx.tm >= 8) {
        if (rdx.pend) { rdx_elegir(); rdx.modo = 1; }
        else rdx.modo = 0;
      }
      break;
  }
  rdx_marcarX();
}

static void rdx_paso(uint32_t ms) {
  if (!rdx.ultOk) { rdx.ult = ms; rdx.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdx.ult) >= RDM_TICK_MS && n < 3) { rdx.ult += RDM_TICK_MS; rdx_tick(); n++; }
  if (n == 3) rdx.ult = ms;
}

static uint16_t rdx_colorX(char ch) {
  switch (ch) {
    case 'B': return RD_C(48, 112, 248);
    case 'L': return RD_C(150, 210, 255);
    case 'D': return RD_C(16, 40, 150);
    case 'S': return RD_C(252, 200, 160);
    case 'K': return RD_C(0, 0, 0);
    case 'R': return RD_C(255, 48, 48);
    case 'G': return RD_C(120, 255, 255);
  }
  return 0;
}

// 2x2 steel block: highlight top-left, shadow bottom-right
static uint16_t rdx_bloque(int px, int py) {
  if (px == 0 && py == 0) return RD_C(205, 236, 255);
  if (px == 1 && py == 1) return RD_C(60, 100, 180);
  return RD_C(110, 160, 236);
}

static uint16_t rdx_pixel(int x, int y) {
  // 1) Cannon charge halo
  if (rdx.modo == 2) {
    int dx = x - (rdx.x + 10), dy = y - RDX_MUZZLE_Y, d2 = dx * dx + dy * dy;
    int r = 1 + rdx.tm / 3;                                   // radius 1..5
    if (d2 <= r * r) {
      if (d2 <= (r * r) / 4) return RD_C(215, 248, 255);
      if (d2 <= (r * r) / 2) return RD_C(90, 220, 255);
      if (((x + y + (int)rdx.n) & 1) == 0) return RD_C(40, 120, 255);
    }
  }
  // 2) Plasma ball with trail
  if (rdx.modo == 3) {
    int dx = x - rdx.sx, dy = y - rdx.sy, d2 = dx * dx + dy * dy;
    if (d2 <= 2) return RD_C(235, 252, 255);
    if (d2 <= 5) return RD_C(90, 220, 255);
    if (d2 <= 10) return RD_C(40, 100, 255);
    dx = x - (rdx.sx - 3); dy = y - (rdx.sy + 3);
    if (dx * dx + dy * dy <= 1) return RD_C(90, 220, 255);
    dx = x - (rdx.sx - 5); dy = y - (rdx.sy + 5);
    if (dx * dx + dy * dy <= 0) return RD_C(40, 100, 255);
  }
  // 3) Shockwave and flying blocks
  if (rdx.bt) {
    int ix = RDX_DIG_X[rdx.bd] + 5, iy = 14;
    if (rdx.bt <= 5) {
      int dx = x - ix, dy = y - iy, d2 = dx * dx + dy * dy, r = rdx.bt * 2;
      if (rdx.bt <= 2 && d2 <= 9) return RD_C(255, 255, 255);
      if (d2 <= r * r && d2 >= (r - 2) * (r - 2)) return (rdx.bt & 1) ? RD_C(235, 252, 255) : RD_C(90, 220, 255);
    }
    for (int k = 0; k < 35; k++) {
      if (rdx.qx[k] == -999) continue;
      int lx = x - rdx.qx[k], ly = y - rdx.qy[k];
      if ((unsigned)lx < 2 && (unsigned)ly < 2) return rdx_bloque(lx, ly);
    }
  }
  // 4) X
  {
    int lx = x - rdx.x, ly = y - RDX_Y0;
    if ((unsigned)lx < 11 && (unsigned)ly < 13) {
      const char (*spr)[12] = (rdx.modo <= 1 && rdx.anim) ? RDX_RUN : RDX_STAND;
      char ch = (char)pgm_read_byte(&spr[ly][(rdx.dir < 0) ? 10 - lx : lx]);
      if (ch != '.') return rdx_colorX(ch);
    }
  }
  // 5) Block digits (destroyed blocks are hidden; new ones appear row by row)
  for (int i = 0; i < 4; i++) {
    int lx = x - RDX_DIG_X[i];
    if ((unsigned)lx >= 10) continue;
    int ly = y - RDX_DIG_Y;
    if ((unsigned)ly < 14 && rdx.ph[i] != 1) {
      int row = ly >> 1;
      bool hay = (pgm_read_byte(&RDM_FUENTE[rdx.d[i]][row]) & (0x10 >> (lx >> 1))) != 0;
      if (rdx.ph[i] == 2 && rdx.rt[i] < row * 2 + 1) hay = false;
      if (hay) {
        if (rdx.ph[i] == 2 && rdx.rt[i] <= row * 2 + 2) return RD_C(190, 255, 255);   // flash when a block lands
        return rdx_bloque(lx & 1, ly & 1);
      }
    }
    break;
  }
  if (rdx.dp && (unsigned)(x - 62) < 2 && ((unsigned)(y - 4) < 2 || (unsigned)(y - 10) < 2))
    return RD_C(120, 230, 255);

  // 6) Highway: neon above and below, asphalt, and moving stripes
  if (y >= 24) {
    if (y == 24) return RD_C(0, 200, 255);
    if (y == 31) return RD_C(160, 20, 130);
    if (y == 30 && ((x + rdx.sc) & 15) < 8) return RD_C(170, 180, 230);
    return RD_C(26, 28, 50);
  }
  // 7) Night skyline with windows
  {
    int altura = 9 + (rdi_hash(x >> 3, 5, 1) % 13);
    if (y >= 24 - altura) {
      if ((x & 3) == 1 && (y & 3) == 1) {
        uint8_t w = rdi_hash(x >> 2, y >> 2, 9);
        if (w < 22) return RD_C(200, 170, 60);
        if (w < 90) return RD_C(40, 90, 150);
      }
      return ((x & 7) == 0) ? RD_C(16, 22, 66) : RD_C(10, 12, 40);
    }
    int b = y >> 2;
    return RD_C(4 + b * 7, 6 + b * 2, 26 + b * 9);
  }
}

// ────────────────────────────────────────────────────────────────────
// "Bomberman"
// ────────────────────────────────────────────────────────────────────
#define RDB_DIG_Y     1        // top row of bricks (14 pixels high -> through y=14)
#define RDB_Y0        20       // top row of Bomberman (12 pixels high -> feet at y=31)
#define RDB_CY        27       // center row of the explosion
#define RDB_FUSE      26       // fuse ticks after the bomb is placed
static const int16_t RDB_DIG_X[4]   = { 35, 48, 69, 82 };
static const int16_t RDB_PILAR_X[5] = { 4, 20, 60, 100, 116 };   // outside the flame columns

// Sprites. P pink, W white, S face, K black, U blue
static const char RDB_STAND[12][10] PROGMEM = {
  "....P....", "..WWWWW..", ".WWWWWWW.", ".WSSSSSW.", ".WSKSKSW.", ".WSSSSSW.",
  "..WWWWW..", ".UUUUUUU.", "SUUUUUUUS", ".UUUUUUU.", "..UU.UU..", "..PP.PP.."
};
static const char RDB_WALK[12][10] PROGMEM = {
  "....P....", "..WWWWW..", ".WWWWWWW.", ".WSSSSSW.", ".WSKSKSW.", ".WSSSSSW.",
  "..WWWWW..", ".UUUUUUU.", "SUUUUUUUS", ".UUUUUUU.", ".UU...UU.", ".PP...PP."
};
// 8x8 bomb in two frames (pulsing). Y = fuse spark, W = highlight
static const char RDB_BOMBA[2][8][9] PROGMEM = {
  { "........", "...YY...", "..KKKK..", ".KKKKKK.", ".KWKKKK.", ".KKKKKK.", ".KKKKKK.", "..KKKK.." },
  { "...YY...", ".KKKKKK.", "KKKKKKKK", "KWKKKKKK", "KKKKKKKK", "KKKKKKKK", ".KKKKKK.", "..KKKK.." }
};

struct RDBEstado {
  int8_t   d[4], obj[4];
  uint8_t  pend;
  uint8_t  modo;            // 0 walks, 1 moves to the column, 2 places bomb, 3 runs and waits, 4 explosion, 5 pause
  int8_t   bloque, dir, fdir;
  uint8_t  anim, tm, fuse;
  int16_t  x, tx, cx;       // Bomberman corner, destination, and bomb center
  bool     bomba;
  uint8_t  ef, efs;         // digit effect: 0 none, 1 burning bricks, 2 smoke
  int8_t   efd;
  int8_t   pfx[6], pfy[6], pfr[6];   // smoke particles (recalculated each tick)
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDBEstado rdb;

static void rdb_marcarB()      { rdMarcar(rdb.x - 1, 19, 11, 13); }
static void rdb_marcarBomba()  { rdMarcar(rdb.cx - 5, 22, 10, 10); }
static void rdb_marcarFuego()  { rdMarcar(rdb.cx - 16, 23, 33, 9); rdMarcar(rdb.cx - 4, 0, 9, 32); }
static void rdb_marcarEfecto() { rdMarcar(RDB_DIG_X[rdb.efd] - 12, 0, 34, 20); }

static void rdb_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdb.d);
  for (int i = 0; i < 4; i++) rdb.obj[i] = rdb.d[i];
  rdb.pend = 0; rdb.modo = 0; rdb.bloque = -1; rdb.dir = 1; rdb.fdir = -1;
  rdb.anim = 0; rdb.tm = 0; rdb.fuse = 0; rdb.bomba = false;
  rdb.x = 20 + (m % 40); rdb.tx = rdb.x; rdb.cx = rdb.x + 4;
  rdb.ef = 0; rdb.efs = 0; rdb.efd = 0;
  for (int j = 0; j < 6; j++) { rdb.pfx[j] = 0; rdb.pfy[j] = 0; rdb.pfr[j] = 0; }
  rdb.dp = (s % 2 == 0); rdb.n = 0; rdb.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rdb_elegir() {
  int mejor = -1, dmin = 9999;
  for (int i = 0; i < 4; i++) {
    if (!(rdb.pend & (1 << i))) continue;
    int dist = abs((RDB_DIG_X[i] + 1) - rdb.x);
    if (dist < dmin) { dmin = dist; mejor = i; }
  }
  rdb.bloque = mejor;
  if (mejor >= 0) rdb.tx = RDB_DIG_X[mejor] + 1;          // its center sits below the center of the digit
}

static void rdb_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rdb.obj[i] = nd[i]; if (nd[i] != rdb.d[i]) mask |= (1 << i); }
  rdb.pend = mask;
  if (mask && rdb.modo == 0) { rdb_elegir(); rdb.modo = 1; }
}

static void rdb_segundo(int s) { rdb.dp = (s % 2 == 0); rdMarcar(62, 3, 2, 10); }

// Smoke particle positions and radii by effect tick
static void rdb_humo() {
  int t = rdb.efs;
  int base = (t < 7) ? 2 + t : (t < 11 ? 9 : 9 - (t - 10));
  for (int j = 0; j < 6; j++) {
    uint8_t h1 = rdi_hash(rdb.efd * 6 + j, 3, 5), h2 = rdi_hash(j, rdb.efd, 11);
    rdb.pfx[j] = RDB_DIG_X[rdb.efd] + 5 + (int)(h1 & 15) - 8;
    rdb.pfy[j] = 8 + (int)(h2 & 7) - 4 - t / 3;
    int r = base - (int)((h2 >> 3) & 1) * 2;
    rdb.pfr[j] = (r < 1) ? 1 : r;
  }
}

static void rdb_tick() {
  rdb_marcarB();
  rdb.n++;

  // Digit effect: burning bricks followed by smoke (the new digit appears inside)
  if (rdb.ef) {
    rdb_marcarEfecto();
    rdb.efs++;
    if (rdb.ef == 1) {
      if (rdb.efs >= 4) { rdb.ef = 2; rdb.efs = 0; rdb_humo(); }
    } else {
      if (rdb.efs == 6) { int i = rdb.efd; rdb.d[i] = rdb.obj[i]; }
      if (rdb.efs >= 18) rdb.ef = 0; else rdb_humo();
    }
    rdb_marcarEfecto();
  }

  switch (rdb.modo) {
    case 0:                                         // walk
      if ((rdb.n & 1) == 0) {
        rdb.x += rdb.dir;
        if (rdb.x >= 115) rdb.dir = -1;
        if (rdb.x <= 3)   rdb.dir = 1;
      }
      if ((rdb.n % 3) == 0) rdb.anim ^= 1;
      break;

    case 1:                                         // moves to the digit column
      if (rdb.x < rdb.tx)      { rdb.x += (rdb.tx - rdb.x >= 2) ? 2 : 1; rdb.dir = 1; }
      else if (rdb.x > rdb.tx) { rdb.x -= (rdb.x - rdb.tx >= 2) ? 2 : 1; rdb.dir = -1; }
      if ((rdb.n % 2) == 0) rdb.anim ^= 1;
      if (rdb.x == rdb.tx) {                        // places the bomb
        rdb.modo = 2; rdb.tm = 0; rdb.anim = 0;
        rdb.bomba = true; rdb.cx = rdb.x + 4; rdb.fuse = 0;
        rdb_marcarBomba();
      }
      break;

    case 2:                                         // crouches beside the bomb briefly
      rdb.tm++; rdb.fuse++; rdb_marcarBomba();
      if (rdb.tm >= 3) { rdb.modo = 3; rdb.tm = 0; rdb.fdir = (rdb.cx < 64) ? -1 : 1; }
      break;

    case 3:                                         // runs away (20 px) and waits for it to explode
      rdb.fuse++; rdb_marcarBomba();
      if (rdb.tm < 10) { rdb.x += 2 * rdb.fdir; rdb.tm++; if ((rdb.n % 2) == 0) rdb.anim ^= 1; }
      else rdb.anim = 0;
      if (rdb.fuse >= RDB_FUSE) {
        rdb.bomba = false; rdb.modo = 4; rdb.tm = 0; rdb.anim = 0;
        rdb_marcarFuego();
      }
      break;

    case 4: {                                       // cross-shaped explosion
      rdb.tm++;
      rdb_marcarFuego();
      if (rdb.tm == 3) {                            // the vertical arm reaches the digit
        int b = rdb.bloque;
        rdb.ef = 1; rdb.efs = 0; rdb.efd = b;
        rdb.pend &= ~(1 << b);
        rdb_marcarEfecto();
      }
      if (rdb.tm >= 12) { rdb.modo = 5; rdb.tm = 0; }
    } break;

    case 5:                                         // pause, then next digit
      rdb.tm++;
      if (rdb.tm >= 6) {
        if (rdb.pend) { rdb_elegir(); rdb.modo = 1; }
        else rdb.modo = 0;
      }
      break;
  }
  rdb_marcarB();
}

static void rdb_paso(uint32_t ms) {
  if (!rdb.ultOk) { rdb.ult = ms; rdb.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdb.ult) >= RDM_TICK_MS && n < 3) { rdb.ult += RDM_TICK_MS; rdb_tick(); n++; }
  if (n == 3) rdb.ult = ms;
}

static uint16_t rdb_colorB(char ch) {
  switch (ch) {
    case 'P': return RD_C(255, 120, 180);
    case 'W': return RD_C(240, 240, 250);
    case 'S': return RD_C(252, 200, 160);
    case 'K': return RD_C(0, 0, 0);
    case 'U': return RD_C(40, 80, 220);
  }
  return 0;
}

static uint16_t rdb_pixel(int x, int y) {
  // 1) Cross-shaped fire: yellow core, orange center, red edge
  if (rdb.modo == 4) {
    int tm = rdb.tm;
    int g = (tm <= 4) ? tm * 25 : (tm <= 8 ? 100 : (12 - tm) * 25);
    if (g > 0) {
      int Lh = 14 * g / 100, Lv = 27 * g / 100;
      int dx = x - rdb.cx, dy = y - RDB_CY, a = -1;
      if (abs(dy) <= 3 && abs(dx) <= Lh) a = abs(dy);
      if (abs(dx) <= 3 && dy <= 3 && dy >= -Lv) { int b = abs(dx); if (a < 0 || b < a) a = b; }
      if (a >= 0) return (a <= 1) ? RD_C(255, 244, 140) : (a == 2) ? RD_C(252, 150, 30) : RD_C(220, 50, 20);
    }
  }
  // 2) Smoke
  if (rdb.ef == 2) {
    for (int j = 0; j < 6; j++) {
      int dx = x - rdb.pfx[j], dy = y - rdb.pfy[j], r = rdb.pfr[j], d2 = dx * dx + dy * dy;
      if (d2 > r * r) continue;
      if (rdb.efs >= 11 && (((x + y) & 1) == 0)) continue;     // fades lighter
      return (d2 > (r - 1) * (r - 1)) ? RD_C(170, 170, 185) : RD_C(232, 232, 240);
    }
  }
  // 3) Bomberman
  {
    int lx = x - rdb.x, ly = y - RDB_Y0;
    if ((unsigned)lx < 9 && (unsigned)ly < 12) {
      bool anda = (rdb.modo == 0 || rdb.modo == 1 || (rdb.modo == 3 && rdb.tm < 10));
      const char (*spr)[10] = (anda && rdb.anim) ? RDB_WALK : RDB_STAND;
      char ch = (char)pgm_read_byte(&spr[ly][lx]);
      if (ch != '.') return rdb_colorB(ch);
    }
  }
  // 4) Pulsing bomb; flashes white at the end
  if (rdb.bomba) {
    int lx = x - (rdb.cx - 4), ly = y - 23;
    if ((unsigned)lx < 8 && (unsigned)ly < 8) {
      char ch = (char)pgm_read_byte(&RDB_BOMBA[(rdb.fuse / 3) & 1][ly][lx]);
      if (ch == 'Y') return (rdb.n & 1) ? RD_C(252, 230, 60) : RD_C(252, 130, 30);
      if (ch == 'W') return RD_C(230, 230, 255);
      if (ch == 'K') return (rdb.fuse >= RDB_FUSE - 6 && (rdb.fuse & 1)) ? RD_C(235, 235, 245) : RD_C(24, 24, 40);
    }
  }
  // 5) Brick digits
  for (int i = 0; i < 4; i++) {
    int lx = x - RDB_DIG_X[i];
    if ((unsigned)lx >= 10) continue;
    int ly = y - RDB_DIG_Y;
    if ((unsigned)ly < 14) {
      bool oculto = (rdb.ef == 2 && rdb.efd == i && rdb.efs < 6);
      if (!oculto && (pgm_read_byte(&RDM_FUENTE[rdb.d[i]][ly >> 1]) & (0x10 >> (lx >> 1)))) {
        if (rdb.ef == 1 && rdb.efd == i) return (rdb.efs & 1) ? RD_C(255, 255, 255) : RD_C(255, 150, 40);
        int px = lx & 1, py = ly & 1;
        if (px == 0 && py == 0) return RD_C(255, 190, 120);
        if (px == 1 && py == 1) return RD_C(130, 60, 30);
        return RD_C(205, 110, 60);
      }
    }
    break;
  }
  if (rdb.dp && (unsigned)(x - 62) < 2 && ((unsigned)(y - 4) < 2 || (unsigned)(y - 10) < 2))
    return RD_C(255, 120, 200);

  // 6) Gray pillars (light top, dark front)
  if (y >= 16 && y <= 23) {
    for (int k = 0; k < 5; k++) {
      int lx = x - RDB_PILAR_X[k];
      if ((unsigned)lx >= 8) continue;
      if (lx == 0 || lx == 7 || y == 16 || y == 23) return RD_C(60, 60, 80);
      return (y <= 19) ? RD_C(200, 200, 214) : RD_C(110, 110, 132);
    }
  }
  // 7) 8x8 checkerboard floor
  return (((x >> 3) + (y >> 3)) & 1) ? RD_C(44, 168, 52) : RD_C(36, 150, 44);
}

// ────────────────────────────────────────────────────────────────────
// "StreetsOfRage"
// ────────────────────────────────────────────────────────────────────
#define RDR_DIG_Y     14       // top row of crates (14 pixels high -> through y=27)
#define RDR_Y0        18       // top row of the fighter (14 pixels high -> feet at y=31)
static const int16_t RDR_DIG_X[4] = { 18, 44, 71, 97 };            // 16-pixel gaps to the left
// Combo, one pose every 2 ticks: arm length and height, leg length
static const uint8_t RDR_ARM_L[16]  = { 0, 6, 6, 2, 6, 6, 2, 5, 6, 2, 2, 2, 2, 2, 2, 2 };
static const int8_t  RDR_ARM_DY[16] = { 1, 0, 0, 1,-1,-1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
static const uint8_t RDR_LEG_L[16]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 8, 8, 3, 0 };
static const int8_t  RDR_DROP[10]   = { -18, -14, -10, -6, -2, 0, -2, 0, -1, 0 };   // fall with bounce
static const uint8_t RDR_LETRA[3][5] PROGMEM = { {6,5,6,5,6}, {2,5,7,5,5}, {6,5,6,5,5} };   // B A R

// 9x14 fighter (facing right); combo arms and leg are drawn in code.
// Y hair, S skin, K eye, W shirt, B belt and boots, J jeans
static const char RDR_STAND[14][10] PROGMEM = {
  "..YYYYY..", ".YYYYYYY.", ".YYSSSSS.", ".YSSKSSS.", "..SSSSSS.", "...SSSS..", "..WWWWW..",
  ".WWWWWWW.", ".WWWWWWW.", "..BBBBB..", "..JJ.JJ..", "..JJ.JJ..", "..JJ.JJ..", ".BBB.BBB."
};
static const char RDR_WALK[14][10] PROGMEM = {
  "..YYYYY..", ".YYYYYYY.", ".YYSSSSS.", ".YSSKSSS.", "..SSSSSS.", "...SSSS..", "..WWWWW..",
  ".WWWWWWW.", ".WWWWWWW.", "..BBBBB..", "..JJ.JJ..", ".JJ...JJ.", ".JJ...JJ.", "BBB...BBB"
};

struct RDREstado {
  int8_t   d[4], obj[4];
  uint8_t  pend;
  uint8_t  modo;            // 0 guards, 1 walks to target, 2 combo, 3 pause
  int8_t   bloque, dir;
  int8_t   efd, bod;        // stack being hit and its previous value
  uint8_t  anim, tm, shk, spk, bt, dph, dt, neon;
  int16_t  x, tx;
  int16_t  qx[35], qy[35];  // flying crates (-999 = none)
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDREstado rdr;

static void rdr_marcarF()  { rdMarcar(rdr.x - 2, 16, 22, 16); }
static void rdr_marcarEf() { rdMarcar(RDR_DIG_X[rdr.efd] - 4, 0, 72, 32); }

static void rdr_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdr.d);
  for (int i = 0; i < 4; i++) rdr.obj[i] = rdr.d[i];
  rdr.pend = 0; rdr.modo = 0; rdr.bloque = -1; rdr.dir = 1; rdr.efd = 0; rdr.bod = 0;
  rdr.anim = 0; rdr.tm = 0; rdr.shk = 0; rdr.spk = 0; rdr.bt = 0; rdr.dph = 0; rdr.dt = 0; rdr.neon = 3;
  rdr.x = RDR_DIG_X[1] - 16; rdr.tx = rdr.x;
  for (int k = 0; k < 35; k++) { rdr.qx[k] = -999; rdr.qy[k] = 0; }
  rdr.dp = (s % 2 == 0); rdr.n = 0; rdr.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rdr_elegir() {
  int mejor = -1, dmin = 9999;
  for (int i = 0; i < 4; i++) {
    if (!(rdr.pend & (1 << i))) continue;
    int dist = abs((RDR_DIG_X[i] - 16) - rdr.x);
    if (dist < dmin) { dmin = dist; mejor = i; }
  }
  rdr.bloque = mejor;
  if (mejor >= 0) rdr.tx = RDR_DIG_X[mejor] - 16;          // fist reaches the stack
}

static void rdr_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rdr.obj[i] = nd[i]; if (nd[i] != rdr.d[i]) mask |= (1 << i); }
  rdr.pend = mask;
  if (mask && rdr.modo == 0) { rdr_elegir(); rdr.modo = 1; }
}

static void rdr_segundo(int s) { rdr.dp = (s % 2 == 0); rdMarcar(61, 17, 2, 8); }

// Crates from the broken stack fly right and upward, then fall under gravity
static void rdr_escombros() {
  int i = rdr.efd, t = rdr.bt;
  for (int k = 0; k < 35; k++) {
    int cx = k % 5, cy = k / 5;
    if (!(pgm_read_byte(&RDM_FUENTE[rdr.bod][cy]) & (0x10 >> cx))) { rdr.qx[k] = -999; continue; }
    int sx = RDR_DIG_X[i] + cx * 2, sy = RDR_DIG_Y + cy * 2;
    uint8_t hh = rdi_hash(i * 35 + k, 13, 7);
    int vx = 8 + (int)(hh & 7);
    int vy = -(10 + (int)((hh >> 3) & 7) * 2);
    rdr.qx[k] = sx + (vx * t) / 8;
    rdr.qy[k] = sy + (vy * t) / 8 + (t * t) / 6;
  }
}

static void rdr_tick() {
  rdr_marcarF();
  rdr.n++;

  // Neon sign: occasionally flashes as a whole, and the A flashes independently
  uint8_t ne = ((((rdr.n / 4) % 41) != 20) ? 1 : 0) | ((((rdr.n / 3) % 17) != 5) ? 2 : 0);
  if (ne != rdr.neon) { rdr.neon = ne; rdMarcar(2, 3, 15, 7); }

  if (rdr.shk) { rdr.shk--; rdr_marcarEf(); }
  if (rdr.spk) { rdr.spk--; rdr_marcarEf(); }

  // New digit falls in (with bounce and dust)
  if (rdr.dph == 2) {
    rdr_marcarEf();
    rdr.dt++;
    if (rdr.dt >= 10) rdr.dph = 0;
    rdr_marcarEf();
  }
  // Crates fly out; the new digit starts falling after 6 ticks
  if (rdr.bt) {
    rdr_marcarEf();
    rdr.bt++;
    if (rdr.bt == 6) { int i = rdr.efd; rdr.d[i] = rdr.obj[i]; rdr.dph = 2; rdr.dt = 0; }
    if (rdr.bt > 22) { rdr.bt = 0; for (int k = 0; k < 35; k++) rdr.qx[k] = -999; }
    else rdr_escombros();
    rdr_marcarEf();
  }

  switch (rdr.modo) {
    case 0:                                         // guard stance: breathes
      if ((rdr.n & 7) == 0) rdr.anim ^= 1;
      break;

    case 1:                                         // walks (3 px per tick) to the gap in the stack
      if (rdr.x < rdr.tx)      { rdr.x += (rdr.tx - rdr.x >= 3) ? 3 : (rdr.tx - rdr.x); rdr.dir = 1; }
      else if (rdr.x > rdr.tx) { rdr.x -= (rdr.x - rdr.tx >= 3) ? 3 : (rdr.x - rdr.tx); rdr.dir = -1; }
      if ((rdr.n % 2) == 0) rdr.anim ^= 1;
      if (rdr.x == rdr.tx) { rdr.modo = 2; rdr.tm = 0; rdr.dir = 1; rdr.anim = 0; rdr.efd = rdr.bloque; }
      break;

    case 2:                                         // combo: 3 punches and a kick
      rdr.tm++;
      if (rdr.tm == 4 || rdr.tm == 10 || rdr.tm == 16) {     // punch connects
        rdr.shk = 4; rdr.spk = 2; rdr_marcarEf();
      }
      if (rdr.tm == 24) {                                     // kick sends the crates flying
        int b = rdr.bloque;
        rdr.bod = rdr.d[b]; rdr.dph = 1; rdr.bt = 1;
        rdr.pend &= ~(1 << b);
        rdr.shk = 0; rdr.spk = 3;
        rdr_escombros(); rdr_marcarEf();
      }
      if (rdr.tm >= 32) { rdr.modo = 3; rdr.tm = 0; }
      break;

    case 3:                                         // pause, then next stack
      rdr.tm++;
      if ((rdr.n & 7) == 0) rdr.anim ^= 1;
      if (rdr.tm >= 6) {
        if (rdr.pend) { rdr_elegir(); rdr.modo = 1; }
        else rdr.modo = 0;
      }
      break;
  }
  rdr_marcarF();
}

static void rdr_paso(uint32_t ms) {
  if (!rdr.ultOk) { rdr.ult = ms; rdr.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdr.ult) >= RDM_TICK_MS && n < 3) { rdr.ult += RDM_TICK_MS; rdr_tick(); n++; }
  if (n == 3) rdr.ult = ms;
}

static uint16_t rdr_colorF(char ch) {
  switch (ch) {
    case 'Y': return RD_C(252, 208, 64);
    case 'S': return RD_C(252, 200, 160);
    case 'K': return RD_C(0, 0, 0);
    case 'W': return RD_C(236, 236, 248);
    case 'B': return RD_C(110, 60, 30);
    case 'J': return RD_C(48, 90, 210);
  }
  return 0;
}

// 2x2 wooden crate: highlight top-left, shadow bottom-right
static uint16_t rdr_caja(int px, int py) {
  if (px == 0 && py == 0) return RD_C(245, 205, 130);
  if (px == 1 && py == 1) return RD_C(110, 66, 30);
  return RD_C(200, 140, 70);
}

static uint16_t rdr_pixel(int x, int y) {
  // 1) Hit spark
  if (rdr.spk) {
    int dx = x - (RDR_DIG_X[rdr.efd] - 2), dy = y - 25;
    if ((dy == 0 && abs(dx) <= 2) || (dx == 0 && abs(dy) <= 2) || (abs(dx) == 1 && abs(dy) == 1))
      return (rdr.n & 1) ? RD_C(255, 255, 255) : RD_C(255, 230, 80);
  }
  // 2) Fighter: body + combo arm and leg
  {
    int bob = (rdr.modo == 0 || rdr.modo == 3) ? (rdr.anim & 1) : 0;
    int lx = x - rdr.x, u = (rdr.dir < 0) ? 8 - lx : lx, v = y - (RDR_Y0 - bob);
    int L = 1, dy = 1, leg = 0;
    if (rdr.modo == 2) { int p = rdr.tm >> 1; if (p > 15) p = 15; L = RDR_ARM_L[p]; dy = RDR_ARM_DY[p]; leg = RDR_LEG_L[p]; }
    if (leg) {
      if (v >= 8 && v <= 9 && u >= 5 && u < 5 + leg) return RD_C(48, 90, 210);
      if (v >= 7 && v <= 9 && u >= 5 + leg && u < 8 + leg) return RD_C(110, 60, 30);
    }
    if (v >= 7 + dy && v <= 8 + dy && u >= 7 && u < 7 + L) return RD_C(252, 200, 160);
    if (v >= 6 + dy && v <= 8 + dy && u >= 7 + L && u < 10 + L) return RD_C(240, 160, 110);
    if ((unsigned)u < 9 && (unsigned)v < 14) {
      const char (*spr)[10] = (rdr.modo == 1 && rdr.anim) ? RDR_WALK : RDR_STAND;
      char ch = (char)pgm_read_byte(&spr[v][u]);
      if (ch != '.') return rdr_colorF(ch);
    }
  }
  // 3) Flying crates
  if (rdr.bt) {
    for (int k = 0; k < 35; k++) {
      if (rdr.qx[k] == -999) continue;
      int lx = x - rdr.qx[k], ly = y - rdr.qy[k];
      if ((unsigned)lx < 2 && (unsigned)ly < 2) return rdr_caja(lx, ly);
    }
  }
  // 4) Crate stacks (shake when hit, fall with bounce and dust)
  for (int i = 0; i < 4; i++) {
    int off = (i == rdr.efd && rdr.shk) ? ((rdr.shk & 1) ? 1 : -1) : 0;
    int lx = x - RDR_DIG_X[i] - off;
    if ((unsigned)(x - RDR_DIG_X[i] + 1) >= 12) continue;
    bool cae = (i == rdr.efd && rdr.dph == 2);
    if (cae && rdr.dt >= 5 && rdr.dt <= 8 && y >= 26 && y <= 29 && (unsigned)(x - RDR_DIG_X[i] + 2) < 14 &&
        rdi_hash(x, y, rdr.dt) < 110) return RD_C(205, 205, 215);                    // landing dust
    if (i == rdr.efd && rdr.dph == 1) break;
    int ly = y - RDR_DIG_Y - (cae ? RDR_DROP[rdr.dt] : 0);
    if ((unsigned)lx < 10 && (unsigned)ly < 14 &&
        (pgm_read_byte(&RDM_FUENTE[rdr.d[i]][ly >> 1]) & (0x10 >> (lx >> 1))))
      return rdr_caja(lx & 1, ly & 1);
    break;
  }
  if (rdr.dp && (unsigned)(x - 61) < 2 && ((unsigned)(y - 18) < 2 || (unsigned)(y - 23) < 2))
    return RD_C(255, 230, 130);

  // 5) "BAR" neon sign at upper left
  if (x >= 2 && x <= 16 && y >= 3 && y <= 9) {
    int lx = x - 4, ly = y - 4;
    if ((unsigned)lx < 11 && (unsigned)ly < 5 && (lx & 3) < 3) {
      int g = lx >> 2;
      if (pgm_read_byte(&RDR_LETRA[g][ly]) & (4 >> (lx & 3))) {
        bool on = (rdr.neon & 1) && (g != 1 || (rdr.neon & 2));
        return on ? RD_C(255, 70, 170) : RD_C(90, 30, 70);
      }
    }
    return RD_C(30, 12, 40);
  }
  // 6) Streetlight (post and head)
  if (x >= 120 && x <= 121 && y >= 8 && y <= 27) return RD_C(70, 70, 90);
  if (x >= 117 && x <= 124 && y >= 5 && y <= 7) return (y == 7) ? RD_C(255, 240, 160) : RD_C(70, 70, 90);

  // 7) Brick wall with the streetlight beam
  if (y <= 27) {
    int row = y >> 2, off = (row & 1) * 4;
    int r, g, b;
    if ((y & 3) == 3 || (((x + off) & 7) == 7)) { r = 30; g = 22; b = 46; }
    else {
      int v = rdi_hash((x + off) >> 3, row, 3) & 3;
      r = 52 + v * 4; g = 40 + v * 3; b = 78 + v * 4;
    }
    if (y >= 8 && abs(x - 121) <= (y - 7) / 2) { r += 36; g += 30; b += 20; }     // light beam
    return RD_C(r, g, b);
  }
  // 8) Curb and asphalt
  if (y == 28) return RD_C(160, 160, 180);
  if (y == 29) return RD_C(110, 110, 130);
  if (y == 31 && (((x >> 3) & 1) == 0)) return RD_C(180, 150, 30);
  return RD_C(36, 36, 48);
}

// ────────────────────────────────────────────────────────────────────
// "Castlevania"
// ────────────────────────────────────────────────────────────────────
#define RDV_DIG_Y     1        // top row of digits (through y=14)
#define RDV_BOX_Y     15       // top row of the hunter (13 pixels high; feet at y=27)
#define RDV_MANO_Y    16       // whip starts here and extends upward
static const int16_t RDV_DIG_X[4] = { 35, 48, 69, 82 };

// Colors: 1 hair, 2 skin, 3 tunic, 4 tunic shadow, 5 pants, 6 boots, 7 belt, 8 dark, 9 whip
static const RDRect RDV_PARADO[] PROGMEM = {
  {3,0,6,3,1},{2,1,2,3,1},{5,2,5,4,2},{8,3,1,1,8},
  {3,6,6,4,3},{3,7,2,3,4},{3,9,6,1,7},
  {3,10,3,2,5},{7,10,3,2,5},{2,12,4,1,6},{7,12,5,1,6},
  {8,7,2,3,3},{9,9,2,2,2},{10,10,2,3,9}
};
static const RDRect RDV_ANDA1[] PROGMEM = {
  {3,0,6,3,1},{2,1,2,3,1},{5,2,5,4,2},{8,3,1,1,8},
  {3,6,6,4,3},{3,7,2,3,4},{3,9,6,1,7},
  {2,10,3,2,5},{8,10,3,2,5},{1,12,4,1,6},{8,12,5,1,6},
  {8,7,2,3,3},{9,9,2,2,2},{10,10,2,3,9}
};
static const RDRect RDV_ANDA2[] PROGMEM = {
  {3,0,6,3,1},{2,1,2,3,1},{5,2,5,4,2},{8,3,1,1,8},
  {3,6,6,4,3},{3,7,2,3,4},{3,9,6,1,7},
  {4,10,3,2,5},{6,10,3,2,5},{3,12,4,1,6},{6,12,4,1,6},
  {8,7,2,3,3},{9,9,2,2,2},{10,10,2,3,9}
};
static const RDRect RDV_LATIGO[] PROGMEM = {
  {3,0,6,3,1},{2,1,2,3,1},{5,2,5,4,2},{8,3,1,1,8},
  {3,6,6,4,3},{3,7,2,3,4},{3,9,6,1,7},
  {3,10,3,2,5},{7,10,3,2,5},{2,12,4,1,6},{7,12,5,1,6},
  {8,4,2,3,3},{9,2,2,3,3},{10,1,2,2,2}
};
#define RDV_N(a) ((int)(sizeof(a) / sizeof(a[0])))
// Castle (28x24, origin x=100, y=4). Colors: 1 silhouette, 2 lit window
static const RDRect RDV_CASTILLO[] PROGMEM = {
  {0,14,28,10,1},{2,8,6,16,1},{3,6,4,2,1},{4,4,2,2,1},{11,3,6,21,1},{12,1,4,2,1},{13,0,2,1,1},
  {20,10,6,14,1},{21,8,4,2,1},{22,6,2,2,1},
  {4,11,1,2,2},{13,8,1,2,2},{14,14,1,2,2},{22,14,1,2,2},{6,18,1,2,2},{24,19,1,2,2},{13,19,1,2,2}
};
static const char RDV_MURCIELAGO[2][4][8] PROGMEM = {
  { "X.....X", "XX...XX", ".XXXXX.", "..X.X.." },
  { "..XXX..", ".XXXXX.", "XX.X.XX", "X.....X" }
};
static const char RDV_LLAMA[3][5][4] PROGMEM = {
  { ".X.", "XXX", "XXX", ".X.", "..." },
  { "..X", ".XX", "XXX", "XXX", ".X." },
  { ".X.", ".XX", "XXX", "XXX", ".X." }
};
static const int8_t RDV_ONDA[8] PROGMEM = { 0, 1, 2, 2, 1, 0, -1, -1 };

struct RDVEstado {
  int8_t   d[4], obj[4];
  uint8_t  pend;
  uint8_t  modo;        // 0 waits, 1 walks, 2 moves to digit, 3 whip extends, 4 whip retracts, 5 returns
  int8_t   dir;
  int16_t  x;           // sprite left x
  uint8_t  anim;
  int8_t   bloque;
  int8_t   lat;         // whip length
  uint8_t  exp[4];
  uint8_t  espera;
  int16_t  destino;
  int16_t  bat;         // bat x position (-20 = none)
  uint8_t  rayo;
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDVEstado rdv;

static void rdv_marcarCazador() { rdMarcar(rdv.x - 2, RDV_BOX_Y - 1, 17, 15); }
static void rdv_marcarLatigo()  { rdMarcar(rdv.x + 8, 3, 6, 15); }
static void rdv_marcarDigito(int i) { rdMarcar(RDV_DIG_X[i] - 1, RDV_DIG_Y, 12, 15); }
static void rdv_marcarBat()     { rdMarcar(rdv.bat - 1, 15, 10, 12); }

static void rdv_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdv.d);
  for (int i = 0; i < 4; i++) { rdv.obj[i] = rdv.d[i]; rdv.exp[i] = 0; }
  rdv.pend = 0; rdv.modo = 0; rdv.dir = 1; rdv.x = 8 + (m % 12);
  rdv.anim = 0; rdv.bloque = -1; rdv.lat = 0; rdv.espera = 30 + (m % 25);
  rdv.bat = -20; rdv.rayo = 0;
  rdv.dp = (s % 2 == 0); rdv.n = 0; rdv.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rdv_elegir() {
  int mejor = -1, dmin = 9999;
  for (int i = 0; i < 4; i++) {
    if (!(rdv.pend & (1 << i))) continue;
    int dist = abs((RDV_DIG_X[i] - 5) - rdv.x);
    if (dist < dmin) { dmin = dist; mejor = i; }
  }
  rdv.bloque = mejor;
}

static void rdv_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rdv.obj[i] = nd[i]; if (nd[i] != rdv.d[i] && !rdv.exp[i]) mask |= (1 << i); }
  rdv.pend = mask;
  if (mask && rdv.modo <= 1) { rdv_elegir(); rdv.modo = 2; }
}

static void rdv_segundo(int s) { rdv.dp = (s % 2 == 0); rdMarcar(60, 3, 6, 10); }

static void rdv_siguiente() {
  if (rdv.pend) { rdv_elegir(); rdv.modo = 2; }
  else { rdv.bloque = -1; rdv.destino = 14; rdv.modo = 5; }
}

static void rdv_tick() {
  rdv_marcarCazador();
  rdv.n++;

  // Atmosphere: torches, bat, and lightning
  if ((rdv.n % 3) == 0) { rdMarcar(27, 15, 6, 8); rdMarcar(96, 15, 6, 8); }
  if (rdv.rayo) { rdv.rayo--; rdMarcar(0, 0, RD_W, RD_H); }
  else if ((rdv.n % 900) == 450) { rdv.rayo = 4; rdMarcar(0, 0, RD_W, RD_H); }
  if (rdv.bat > -20) {
    if ((rdv.n & 1) == 0) {
      rdv_marcarBat(); rdv.bat--; if (rdv.bat < -9) rdv.bat = -20; else rdv_marcarBat();
    }
    if ((rdv.n % 4) == 0) rdv_marcarBat();
  } else if ((rdv.n % 700) == 300) { rdv.bat = 127; rdv_marcarBat(); }

  for (int i = 0; i < 4; i++) if (rdv.exp[i]) {          // embers fading out
    rdv.exp[i]--;
    if (!rdv.exp[i]) rdv.d[i] = rdv.obj[i];
    rdv_marcarDigito(i);
  }

  switch (rdv.modo) {
    case 0:                                              // waits
      rdv.anim = 0;
      if (rdv.espera) rdv.espera--;
      if (!rdv.espera) {
        rdv.destino = (rdv.x < 14) ? 22 : 6;
        rdv.dir = (rdv.destino > rdv.x) ? 1 : -1;
        rdv.modo = 1;
      }
      break;
    case 1:                                              // walks around its area
      rdv.x += rdv.dir;
      rdv.anim = 1 + ((rdv.n / 2) & 1);
      if (rdv.x == rdv.destino) { rdv.modo = 0; rdv.anim = 0; rdv.dir = 1; rdv.espera = 40 + (rdv.n % 40); }
      break;
    case 2: {                                            // moves underneath the digit
      int obj = RDV_DIG_X[rdv.bloque] - 5;
      int dx = obj - rdv.x;
      if (dx == 0) { rdv.modo = 3; rdv.lat = 0; rdv.anim = 0; }
      else {
        rdv.dir = dx > 0 ? 1 : -1;
        rdv.x += (abs(dx) >= 2) ? 2 * rdv.dir : rdv.dir;
        rdv.anim = 1 + ((rdv.n / 2) & 1);
      }
    } break;
    case 3:                                              // whip extends
      rdv_marcarLatigo();
      rdv.dir = 1;
      rdv.lat += 4;
      if (rdv.lat >= 11) {
        rdv.lat = 11;
        int b = rdv.bloque;
        rdv.exp[b] = 5; rdv.pend &= ~(1 << b);
        rdv_marcarDigito(b);
        rdv.modo = 4;
      }
      rdv_marcarLatigo();
      break;
    case 4:                                              // whip retracts
      rdv_marcarLatigo();
      rdv.lat -= 6;
      if (rdv.lat <= 0) { rdv.lat = 0; rdv_siguiente(); }
      rdv_marcarLatigo();
      break;
    case 5:                                              // returns to its area
      if (abs(rdv.destino - rdv.x) <= 1) { rdv.x = rdv.destino; rdv.modo = 0; rdv.espera = 40; rdv.anim = 0; rdv.dir = 1; }
      else {
        rdv.dir = (rdv.destino > rdv.x) ? 1 : -1;
        rdv.x += 2 * rdv.dir;
        rdv.anim = 1 + ((rdv.n / 2) & 1);
      }
      break;
  }
  rdv_marcarCazador();
}

static void rdv_paso(uint32_t ms) {
  if (!rdv.ultOk) { rdv.ult = ms; rdv.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdv.ult) >= RDM_TICK_MS && n < 3) { rdv.ult += RDM_TICK_MS; rdv_tick(); n++; }
  if (n == 3) rdv.ult = ms;
}

static uint16_t rdv_colorCazador(uint8_t c) {
  switch (c) {
    case 1: return RD_C(150, 80, 30);
    case 2: return RD_C(240, 190, 140);
    case 3: return RD_C(200, 110, 40);
    case 4: return RD_C(140, 70, 25);
    case 5: return RD_C(50, 70, 170);
    case 6: return RD_C(80, 50, 30);
    case 7: return RD_C(240, 200, 60);
    case 8: return RD_C(16, 12, 20);
    case 9: return RD_C(140, 90, 40);
  }
  return 0;
}

static uint16_t rdv_fondo(int x, int y) {
  // stone floor
  if (y >= 28) {
    if (y == 28) return RD_C(96, 90, 120);
    bool junta = (y == 31) || ((x + ((y == 30) ? 4 : 0)) & 7) == 0;
    return junta ? RD_C(30, 28, 46) : RD_C(62, 58, 84);
  }
  // castle (in front of the moon)
  {
    int lx = x - 100, ly = y - 4;
    if ((unsigned)lx < 28 && (unsigned)ly < 24) {
      uint8_t c = rdRectColor(RDV_CASTILLO, RDV_N(RDV_CASTILLO), lx, ly);
      if (c == 2) return RD_C(255, 200, 60);
      if (c == 1) return RD_C(12, 6, 26);
    }
  }
  // graveyard crosses
  if (y >= 22 && y <= 27) {
    if (x == 3 || x == 57) return RD_C(28, 22, 48);
    if (y == 24 && (x == 2 || x == 4 || x == 56 || x == 58)) return RD_C(28, 22, 48);
  }
  // moon
  {
    int dx = x - 122, dy = y - 6;
    if (dx * dx + dy * dy <= 28) return (dx == -2 && dy == -1) || (dx == 1 && dy == 2) ? RD_C(196, 196, 168) : RD_C(236, 236, 206);
  }
  // stars and sky
  if (((x * 131 + y * 197) % 89) == 0) return RD_C(120, 120, 170);
  return RD_C(6 + y, 4 + (y >> 1), 28 + y * 3);
}

static uint16_t rdv_capas(int x, int y) {
  // 1) Whip
  if (rdv.lat > 0 && y <= RDV_MANO_Y && y > RDV_MANO_Y - rdv.lat) {
    int wx = rdv.x + 10 + ((y & 2) ? 1 : 0);
    if (x == wx) return (y == RDV_MANO_Y - rdv.lat + 1 && rdv.lat >= 11) ? RD_C(255, 255, 210) : RD_C(170, 110, 50);
  }
  // 2) Hunter
  {
    int lx = x - rdv.x, ly = y - RDV_BOX_Y;
    if ((unsigned)lx < 13 && (unsigned)ly < 13) {
      if (rdv.dir < 0) lx = 12 - lx;
      const RDRect* r = RDV_PARADO; int n = RDV_N(RDV_PARADO);
      if (rdv.modo == 3 || rdv.modo == 4)      { r = RDV_LATIGO; n = RDV_N(RDV_LATIGO); }
      else if (rdv.anim == 1)                  { r = RDV_ANDA1;  n = RDV_N(RDV_ANDA1); }
      else if (rdv.anim == 2)                  { r = RDV_ANDA2;  n = RDV_N(RDV_ANDA2); }
      uint8_t c = rdRectColor(r, n, lx, ly);
      if (c) return rdv_colorCazador(c);
    }
  }
  // 3) Bat
  if (rdv.bat > -20) {
    int lx = x - rdv.bat, ly = y - (18 + (int8_t)pgm_read_byte(&RDV_ONDA[((rdv.bat >> 3) & 7)]) * 2);
    if ((unsigned)lx < 7 && (unsigned)ly < 4 &&
        pgm_read_byte(&RDV_MURCIELAGO[(rdv.n >> 2) & 1][ly][lx]) == 'X') return RD_C(110, 90, 150);
  }
  // 4) Fiery clock
  if (y >= RDV_DIG_Y && y <= RDV_DIG_Y + 13 && x >= 35 && x <= 91) {
    int ly = y - RDV_DIG_Y;
    for (int i = 0; i < 4; i++) {
      int lx = x - RDV_DIG_X[i];
      if ((unsigned)lx >= 10) continue;
      if (rdv.exp[i]) {                                  // embers
        static const uint8_t DENS[6] = { 0, 60, 110, 160, 210, 240 };
        unsigned v = (unsigned)x * 7919u + (unsigned)y * 104729u + (unsigned)rdv.exp[i] * 1299709u;
        v ^= v >> 13; v *= 0x5bd1e995u; v ^= v >> 15;
        if ((v & 0xFF) < DENS[rdv.exp[i]]) {
          uint8_t c = (v >> 8) & 3;
          return c == 0 ? RD_C(255, 240, 120) : c == 1 ? RD_C(255, 170, 40) : c == 2 ? RD_C(240, 90, 20) : RD_C(170, 30, 10);
        }
        break;
      }
      if (rdDigito2(rdv.d[i], lx, ly))                   // flame gradient: yellow -> orange -> red
        return RD_C(255, 250 - ly * 12, 170 - ly * 12);
      break;
    }
    if (rdv.dp) {
      int lx = x - 62;
      if ((unsigned)lx < 2 && ((unsigned)(y - 4) < 2 || (unsigned)(y - 10) < 2)) return RD_C(255, 190, 60);
    }
  }
  // 5) Torches
  for (int k = 0; k < 2; k++) {
    int tx = k ? 98 : 29;
    if (x == tx && y >= 20 && y <= 24) return RD_C(90, 70, 50);                      // bracket
    int lx = x - (tx - 1), ly = y - 15;
    if ((unsigned)lx < 3 && (unsigned)ly < 5 &&
        pgm_read_byte(&RDV_LLAMA[(rdv.n / 3 + k) % 3][ly][lx]) == 'X') return ly < 2 ? RD_C(255, 230, 120) : RD_C(255, 140, 30);
  }
  // 6) Background (with lightning)
  uint16_t c = rdv_fondo(x, y);
  if (rdv.rayo) c = rdBlanco(c, rdv.rayo * 2);
  return c;
}

static uint16_t rdv_pixel(int x, int y) { return rdv_capas(x, y); }

// ────────────────────────────────────────────────────────────────────
// "Street Fighter"
// ────────────────────────────────────────────────────────────────────
#define RDC_SPR_Y    9         // top row of the sprite (22 pixels high; feet at y=30)
#define RDC_P1_X     5         // left sprite x (16 pixels wide)
#define RDC_P2_X     107       // right sprite x (faces left)
#define RDC_BOLA_Y   21
#define RDC_VIDA     44
#define RDC_DIG_Y    5
static const int16_t RDC_DIG_X[4] = { 35, 48, 69, 82 };
static const uint8_t RDC_MONTE[16] PROGMEM = { 2,4,6,8,6,5,3,2,3,5,7,8,6,4,3,2 };
static const uint8_t RDC_FLASH_K[7] PROGMEM = { 0, 2, 4, 6, 8, 8, 8 };

// Colors: 1 hair, 2 skin, 3 skin shadow, 4 kimono, 5 kimono shadow, 6 headband, 7 dark, 8 footwear, 9 belt
static const RDRect RDC_GUARDIA[] PROGMEM = {
  {4,13,4,3,4},{3,16,3,4,5},{1,20,5,2,8},
  {8,13,4,3,4},{9,16,3,4,4},{9,20,6,2,8},
  {5,6,7,7,4},{5,7,2,6,5},{7,6,3,2,2},{5,12,7,2,9},
  {3,8,3,3,4},{2,10,3,3,2},
  {6,1,6,5,2},{5,0,7,2,1},{4,1,2,4,1},{5,2,7,1,6},{2,3,3,1,6},{9,3,1,1,7},{10,5,1,1,3},
  {10,7,3,2,4},{12,7,2,3,4},{13,6,3,3,2}
};
static const RDRect RDC_LANZAR[] PROGMEM = {
  {3,13,4,3,4},{2,16,3,4,5},{0,20,5,2,8},
  {9,13,4,3,4},{11,16,3,4,4},{11,20,5,2,8},
  {6,6,7,7,4},{6,7,2,6,5},{8,6,3,2,2},{6,12,7,2,9},
  {7,1,6,5,2},{6,0,7,2,1},{5,1,2,4,1},{6,2,7,1,6},{3,3,3,1,6},{10,3,1,1,7},{11,5,1,1,3},
  {11,10,3,2,5},{13,10,2,2,2},
  {11,7,3,3,4},{13,7,3,4,2}
};
static const RDRect RDC_GOLPEADO[] PROGMEM = {
  {3,13,4,3,4},{2,16,3,4,5},{0,20,5,2,8},
  {7,13,4,3,4},{8,16,3,4,4},{8,20,6,2,8},
  {3,6,7,7,4},{3,7,2,6,5},{5,6,3,2,2},{3,12,7,2,9},
  {3,1,6,5,2},{2,0,7,2,1},{1,1,2,4,1},{2,2,7,1,6},{0,3,2,1,6},{6,3,1,1,7},{7,5,1,1,7},
  {0,7,4,2,4},{0,6,2,2,2},{9,8,3,2,4},{11,7,3,3,2}
};
#define RDC_N(a) ((int)(sizeof(a) / sizeof(a[0])))

struct RDCEstado {
  int8_t   d[4], obj[4];
  uint8_t  vida[2];
  uint8_t  modo;        // 0 idle, 1 charges, 2 fireballs travel, 3 winning fireball, 4 impact
  uint8_t  gana;        // 0 left fighter wins
  uint8_t  t;
  int16_t  b0, b1;      // x center of each fireball
  bool     v0, v1;
  uint8_t  pose[2];     // 0 guards, 1 throws, 2 hit
  uint8_t  bob;
  uint8_t  spark;  int16_t sparkX;
  uint8_t  hitSp;  int16_t hitX;
  uint8_t  flash;
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDCEstado rdc;

static void rdc_marcarLuchadores() {
  rdMarcar(RDC_P1_X - 1, RDC_SPR_Y - 1, 18, 25);
  rdMarcar(RDC_P2_X - 1, RDC_SPR_Y - 1, 18, 25);
}
static void rdc_marcarBolas() {
  rdMarcar(rdc.b0 - 12, RDC_BOLA_Y - 5, 24, 11);
  rdMarcar(rdc.b1 - 12, RDC_BOLA_Y - 5, 24, 11);
}
static void rdc_marcarSpark() { rdMarcar(rdc.sparkX - 23, RDC_BOLA_Y - 23, 47, 47); }
static void rdc_marcarHit() { rdMarcar(rdc.hitX - 11, RDC_BOLA_Y - 11, 23, 23); }

static bool rdc_difiere() {
  for (int i = 0; i < 4; i++) if (rdc.d[i] != rdc.obj[i]) return true;
  return false;
}

static void rdc_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdc.d);
  for (int i = 0; i < 4; i++) rdc.obj[i] = rdc.d[i];
  rdc.vida[0] = RDC_VIDA - (m % 7) * 3;
  rdc.vida[1] = RDC_VIDA - (m % 5) * 3;
  rdc.modo = 0; rdc.gana = m & 1; rdc.t = 0;
  rdc.v0 = rdc.v1 = false; rdc.b0 = 0; rdc.b1 = 0;
  rdc.pose[0] = rdc.pose[1] = 0; rdc.bob = 0;
  rdc.spark = 0; rdc.hitSp = 0; rdc.flash = 0;
  rdc.dp = (s % 2 == 0); rdc.n = 0; rdc.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rdc_cambioMinuto(int h, int m) {
  (void)h;
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  for (int i = 0; i < 4; i++) rdc.obj[i] = nd[i];
  rdc.gana = m & 1;
}

static void rdc_segundo(int s) { rdc.dp = (s % 2 == 0); rdMarcar(60, 6, 8, 11); }

static void rdc_tick() {
  rdc.n++;

  if ((rdc.n % 10) == 0 && rdc.pose[0] == 0 && rdc.pose[1] == 0) { rdc.bob ^= 1; rdc_marcarLuchadores(); }

  if (rdc.flash) {
    rdc.flash--;
    rdMarcar(0, 0, RD_W, RD_H);
    if (rdc.flash == 3 && (rdc.vida[0] == 0 || rdc.vida[1] == 0)) {   // K.O.: next round begins under the flash
      rdc.vida[0] = rdc.vida[1] = RDC_VIDA;
    }
  }
  if (rdc.spark) { rdc_marcarSpark(); rdc.spark--; rdc_marcarSpark(); }
  if (rdc.hitSp) { rdc_marcarHit();   rdc.hitSp--; rdc_marcarHit(); }

  switch (rdc.modo) {
    case 0:
      if (rdc_difiere() && !rdc.flash) {                 // the exchange begins
        rdc.modo = 1; rdc.t = 0;
        rdc.pose[0] = rdc.pose[1] = 1;
        rdc_marcarLuchadores();
      }
      break;
    case 1:                                              // charges
      rdc.t++;
      if (rdc.t == 6) {
        rdc.b0 = RDC_P1_X + 21; rdc.b1 = RDC_P2_X - 5;
        rdc.v0 = rdc.v1 = true;
        rdc.modo = 2;
        rdc_marcarBolas();
      }
      break;
    case 2:                                              // the two fireballs approach
      rdc_marcarBolas();
      rdc.b0 += 3; rdc.b1 -= 3;
      if (rdc.b0 + 4 >= rdc.b1 - 4) {                    // collision!
        rdc.sparkX = (rdc.b0 + rdc.b1) / 2;
        rdc.spark = 10;
        rdc.flash = 4;                                   // flash: digits change underneath
        for (int i = 0; i < 4; i++) rdc.d[i] = rdc.obj[i];
        if (rdc.gana == 0) { rdc.v1 = false; rdc.b0 = rdc.sparkX; }
        else               { rdc.v0 = false; rdc.b1 = rdc.sparkX; }
        rdc.pose[rdc.gana] = 1; rdc.pose[1 - rdc.gana] = 0;
        rdc.modo = 3;
        rdMarcar(0, 0, RD_W, RD_H);
      }
      rdc_marcarBolas();
      break;
    case 3: {                                            // winning fireball continues to the opponent
      rdc_marcarBolas();
      bool llega;
      if (rdc.gana == 0) { rdc.b0 += 3; llega = (rdc.b0 >= RDC_P2_X + 2); }
      else               { rdc.b1 -= 3; llega = (rdc.b1 <= RDC_P1_X + 14); }
      if (llega) {
        int def = 1 - rdc.gana;
        rdc.v0 = rdc.v1 = false;
        rdc.pose[def] = 2; rdc.pose[rdc.gana] = 0;
        rdc.hitSp = 5; rdc.hitX = def ? RDC_P2_X + 3 : RDC_P1_X + 13;
        uint8_t dmg = (rdc.vida[def] < 4) ? rdc.vida[def] : 4;
        rdc.vida[def] -= dmg;
        rdMarcar(0, 0, RD_W, 4);
        rdc.modo = 4; rdc.t = 0;
        rdc_marcarLuchadores(); rdc_marcarHit();
      }
      rdc_marcarBolas();
    } break;
    case 4:                                              // opponent reels
      rdc.t++;
      if (rdc.t == 8) {
        if (rdc.vida[0] == 0 || rdc.vida[1] == 0) { rdc.flash = 6; rdMarcar(0, 0, RD_W, RD_H); }   // K.O.
        rdc.pose[0] = rdc.pose[1] = 0;
        rdc.modo = 0;
        rdc_marcarLuchadores();
      }
      break;
  }
}

static void rdc_paso(uint32_t ms) {
  if (!rdc.ultOk) { rdc.ult = ms; rdc.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdc.ult) >= RDM_TICK_MS && n < 3) { rdc.ult += RDM_TICK_MS; rdc_tick(); n++; }
  if (n == 3) rdc.ult = ms;
}

static uint16_t rdc_color(uint8_t c, uint8_t p) {
  switch (c) {
    case 1: return p ? RD_C(250, 200, 60) : RD_C(40, 24, 20);
    case 2: return RD_C(245, 190, 140);
    case 3: return RD_C(200, 140, 100);
    case 4: return p ? RD_C(215, 40, 40) : RD_C(245, 245, 255);
    case 5: return p ? RD_C(150, 20, 30) : RD_C(170, 175, 205);
    case 6: return p ? RD_C(250, 220, 0) : RD_C(230, 30, 30);
    case 7: return RD_C(20, 16, 24);
    case 8: return RD_C(60, 40, 30);
    case 9: return RD_C(40, 40, 40);
  }
  return 0;
}

static uint16_t rdc_luchador(int x, int y, int p) {
  int lx = x - (p ? RDC_P2_X : RDC_P1_X);
  int ly = y - (RDC_SPR_Y + (rdc.pose[p] == 0 ? rdc.bob : 0));
  if ((unsigned)lx >= 16 || (unsigned)ly >= 22) return 0;
  if (p) lx = 15 - lx;
  uint8_t c;
  switch (rdc.pose[p]) {
    case 1:  c = rdRectColor(RDC_LANZAR,   RDC_N(RDC_LANZAR),   lx, ly); break;
    case 2:  c = rdRectColor(RDC_GOLPEADO, RDC_N(RDC_GOLPEADO), lx, ly); break;
    default: c = rdRectColor(RDC_GUARDIA,  RDC_N(RDC_GUARDIA),  lx, ly); break;
  }
  return c ? rdc_color(c, p) : 0;
}

static uint16_t rdc_bola(int x, int y, int cx, int dir) {
  int dx = (x - cx) * dir, dy = y - RDC_BOLA_Y;
  int d2 = dx * dx + dy * dy;
  if (dx >= -3) {
    if (d2 <= 2) return RD_C(255, 255, 255);
    if (d2 <= 6) return RD_C(160, 240, 255);
    if (d2 <= 12) return RD_C(40, 130, 255);
  }
  if (dx < 0 && dx >= -9) {
    int ady = dy < 0 ? -dy : dy;
    if (ady <= 2 - ((-dx) >> 2)) return (dx > -5) ? RD_C(90, 190, 255) : RD_C(30, 90, 220);
  }
  return 0;
}

static bool rdc_star(int dx, int dy, int age, int r) {
  int ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy;
  int d = (ax > ay ? ax : ay) + ((ax > ay ? ay : ax) >> 1);
  if (d > r) return false;
  if (d >= r - 1) return true;
  if (ax <= 1 || ay <= 1 || (ax - ay <= 1 && ay - ax <= 1)) return true;
  return d <= 6 - age;
}
static uint16_t rdc_colorChispa(int age) {
  return age < 3 ? RD_C(255, 255, 255) : age < 6 ? RD_C(255, 230, 60) : RD_C(255, 140, 30);
}

static uint16_t rdc_capas(int x, int y) {
  // 1) Collision and impact sparks
  if (rdc.spark) {
    int age = 10 - rdc.spark;
    if (rdc_star(x - rdc.sparkX, y - RDC_BOLA_Y, age, 3 + age * 2)) return rdc_colorChispa(age);
  }
  if (rdc.hitSp) {
    int age = 5 - rdc.hitSp;
    if (rdc_star(x - rdc.hitX, y - RDC_BOLA_Y, age, 2 + age * 2)) return rdc_colorChispa(age + 2);
  }
  // 2) Energy balls
  if (rdc.v0) { uint16_t c = rdc_bola(x, y, rdc.b0, 1);  if (c) return c; }
  if (rdc.v1) { uint16_t c = rdc_bola(x, y, rdc.b1, -1); if (c) return c; }
  // 3) Health bar
  if (y < 4) {
    bool izq = (x >= 1 && x <= 46), der = (x >= 81 && x <= 126);
    if (izq || der) {
      if (y == 0 || y == 3) return RD_C(10, 10, 20);
      if (x == 1 || x == 46 || x == 81 || x == 126) return RD_C(10, 10, 20);
      bool lleno = izq ? (x >= 46 - rdc.vida[0]) : (x < 82 + rdc.vida[1]);
      if (lleno) return (y == 1) ? RD_C(255, 240, 80) : RD_C(240, 180, 0);
      return RD_C(190, 20, 20);
    }
  }
  // 4) Fighters
  { uint16_t c = rdc_luchador(x, y, 0); if (c) return c; }
  { uint16_t c = rdc_luchador(x, y, 1); if (c) return c; }
  // 5) Clock: large digits with shadow and colon
  if (y >= RDC_DIG_Y && y <= RDC_DIG_Y + 14 && x >= 35 && x <= 93) {
    for (int i = 0; i < 4; i++) {
      int lx = x - RDC_DIG_X[i], ly = y - RDC_DIG_Y;
      if ((unsigned)lx > 10) continue;
      if (rdDigito2(rdc.d[i], lx, ly)) return RD_C(255, 255, 255);
      if (rdDigito2(rdc.d[i], lx - 1, ly - 1)) return RD_C(40, 16, 60);
    }
    if (rdc.dp) {
      int lx = x - 62, ly = y - 8;
      bool pt = ((unsigned)lx < 2) && ((unsigned)ly < 2 || (unsigned)(ly - 6) < 2);
      if (pt) return RD_C(255, 255, 255);
      int sx = lx - 1, sy = ly - 1;
      if (((unsigned)sx < 2) && ((unsigned)sy < 2 || (unsigned)(sy - 6) < 2)) return RD_C(40, 16, 60);
    }
  }
  // 6) Scene: dark scoreboard, sunset sky, mountains, and wooden floor
  if (y >= 28) {
    if (y == 28) return RD_C(150, 100, 60);
    bool junta = ((x + ((y == 30) ? 4 : 0)) & 7) == 0;
    return junta ? RD_C(60, 36, 22) : (y == 31 ? RD_C(70, 44, 28) : RD_C(115, 72, 42));
  }
  if (y < 4) return RD_C(8, 6, 24);
  int top = 24 - (int)pgm_read_byte(&RDC_MONTE[(x >> 2) & 15]);
  if (y >= top) return RD_C(52, 30, 96);
  int t = (y - 4) * 12; if (t > 255) t = 255;
  return RD_C(70 + ((t * 185) >> 8), 40 + ((t * 110) >> 8), 140 - ((t * 70) >> 8));
}

static uint16_t rdc_pixel(int x, int y) {
  uint16_t c = rdc_capas(x, y);
  if (rdc.flash) c = rdBlanco(c, pgm_read_byte(&RDC_FLASH_K[rdc.flash > 6 ? 6 : rdc.flash]));
  return c;
}

// ────────────────────────────────────────────────────────────────────
// "Chrono Trigger"
// ────────────────────────────────────────────────────────────────────
#define RDH_PX        112      // portal center
#define RDH_PY        15
#define RDH_R0        11       // idle radius
#define RDH_DIG_Y     3
#define RDH_HERO_X    12       // traveler sprite left x (14 pixels wide)
#define RDH_HERO_Y    8        // top row (18 pixels high; feet at y=25)
#define RDH_PLAT_Y    26       // first platform row
static const int16_t RDH_DIG_X[4] = { 35, 48, 69, 82 };
static const uint8_t RDH_FLASH_K[5] PROGMEM = { 0, 2, 5, 8, 8 };

// Colors: 1 hair, 2 skin, 3 tunic, 4 tunic shadow, 5 pants, 6 boots, 7 scarf, 8 dark, 9 belt
#define RDH_CUERPO \
  {3,3,9,3,1},{3,0,2,3,1},{6,0,2,3,1},{9,1,2,2,1},{0,3,3,2,1}, \
  {6,5,5,4,2},{9,6,1,1,8},{4,9,8,1,7}, \
  {4,10,8,5,3},{4,11,2,4,4},{4,14,8,1,9}, \
  {11,11,2,3,3},{12,13,2,2,2},{3,11,2,3,4}
static const RDRect RDH_HERO_A[] PROGMEM = { RDH_CUERPO, {0,9,4,2,7}, {4,15,3,2,5},{8,15,3,2,5},{3,17,4,1,6},{8,17,5,1,6} };
static const RDRect RDH_HERO_B[] PROGMEM = { RDH_CUERPO, {0,10,4,2,7},{1,8,2,1,7}, {4,15,3,2,5},{8,15,3,2,5},{3,17,4,1,6},{8,17,5,1,6} };
static const RDRect RDH_HERO_C[] PROGMEM = { RDH_CUERPO, {0,8,5,1,7},{0,9,3,2,7}, {3,15,3,2,5},{9,15,3,2,5},{2,17,4,1,6},{9,17,5,1,6} };
#define RDH_N(a) ((int)(sizeof(a) / sizeof(a[0])))

struct RDHEstado {
  int8_t   d[4], obj[4];
  uint8_t  modo;        // 0 idle, 1 opens, 2 flashes, 3 closes
  uint8_t  t;
  int16_t  R;           // portal radius
  uint8_t  fase;        // vortex rotation
  uint8_t  pose;        // 0 and 1: scarf; 2: braces against the wind
  uint8_t  flash;
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDHEstado rdh;

static void rdh_marcarPortal() {
  int r = rdh.R + 2;
  rdMarcar(RDH_PX - r, RDH_PY - r, 2 * r + 1, 2 * r + 1);
}
static void rdh_marcarHero() { rdMarcar(RDH_HERO_X - 1, RDH_HERO_Y - 1, 16, 20); }

static void rdh_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rdh.d);
  for (int i = 0; i < 4; i++) rdh.obj[i] = rdh.d[i];
  rdh.modo = 0; rdh.t = 0; rdh.R = RDH_R0; rdh.fase = (uint8_t)(m * 9);
  rdh.pose = 0; rdh.flash = 0;
  rdh.dp = (s % 2 == 0); rdh.n = 0; rdh.ultOk = false;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rdh_cambioMinuto(int h, int m) {
  (void)h; (void)m;
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  for (int i = 0; i < 4; i++) rdh.obj[i] = nd[i];
}

static void rdh_segundo(int s) { rdh.dp = (s % 2 == 0); rdMarcar(60, 5, 6, 10); }

static bool rdh_difiere() {
  for (int i = 0; i < 4; i++) if (rdh.d[i] != rdh.obj[i]) return true;
  return false;
}

static void rdh_tick() {
  rdh.n++;
  rdh_marcarPortal();

  if ((rdh.n % 8) == 0) rdMarcar(0, 0, RD_W, RDH_PLAT_Y);        // twinkling stars

  switch (rdh.modo) {
    case 0:
      rdh.fase += 4;
      if ((rdh.n % 12) == 0) { rdh.pose ^= 1; rdh_marcarHero(); }
      if (rdh_difiere()) { rdh.modo = 1; rdh.t = 0; rdh.pose = 2; rdh_marcarHero(); }
      break;
    case 1:                                              // portal opens
      rdh.t++; rdh.fase += 14;
      rdh.R = RDH_R0 + (rdh.t * rdh.t * 5) / 6;
      if (rdh.t >= 12) {
        rdh.R = 140; rdh.modo = 2; rdh.flash = 4;
        for (int i = 0; i < 4; i++) rdh.d[i] = rdh.obj[i];
        rdMarcar(0, 0, RD_W, RD_H);
      }
      break;
    case 2:                                              // white flash
      rdh.fase += 14;
      rdMarcar(0, 0, RD_W, RD_H);
      if (rdh.flash) rdh.flash--;
      if (!rdh.flash) { rdh.modo = 3; rdh.t = 0; }
      break;
    case 3: {                                            // portal closes
      rdh.t++; rdh.fase += 14;
      int k = 12 - rdh.t; if (k < 0) k = 0;
      rdh.R = RDH_R0 + (k * k * 5) / 6;
      if (rdh.t >= 12) { rdh.R = RDH_R0; rdh.modo = 0; rdh.pose = 0; rdh_marcarHero(); }
    } break;
  }
  rdh_marcarPortal();
}

static void rdh_paso(uint32_t ms) {
  if (!rdh.ultOk) { rdh.ult = ms; rdh.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rdh.ult) >= RDM_TICK_MS && n < 3) { rdh.ult += RDM_TICK_MS; rdh_tick(); n++; }
  if (n == 3) rdh.ult = ms;
}

// Approximate angle 0..255 around a point (no trigonometry)
static uint8_t rdh_ang(int dx, int dy) {
  int ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy;
  int s = ax + ay;
  if (s == 0) return 0;
  int p = (ay * 64) / s;
  if (dx >= 0) return (uint8_t)((dy >= 0) ? p : 256 - p);
  return (uint8_t)((dy >= 0) ? 128 - p : 128 + p);
}

// Vortex color at (dx, dy) relative to center, or 0 outside the portal
static uint16_t rdh_vortice(int dx, int dy) {
  int d2 = dx * dx + dy * dy;
  int R = rdh.R;
  if (d2 > R * R) return 0;
  if (R < 100 && d2 >= (R - 1) * (R - 1)) return RD_C(170, 230, 255);          // glowing edge
  int ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy;
  int r = (ax > ay ? ax : ay) + ((ax > ay ? ay : ax) >> 1);
  if (r <= 1) return RD_C(255, 255, 255);
  int v = (int)rdh_ang(dx, dy) * 3 + r * 12 - (int)rdh.fase;
  int tri = (v & 255) - 128; if (tri < 0) tri = -tri;
  if (tri < 30) return RD_C(8, 16, 100);
  if (tri < 60) return RD_C(24, 70, 200);
  if (tri < 95) return RD_C(70, 190, 255);
  return RD_C(240, 252, 255);
}

static uint16_t rdh_color(uint8_t c) {
  switch (c) {
    case 1: return RD_C(235, 70, 40);
    case 2: return RD_C(245, 190, 140);
    case 3: return RD_C(60, 140, 230);
    case 4: return RD_C(30, 90, 170);
    case 5: return RD_C(50, 50, 100);
    case 6: return RD_C(110, 70, 40);
    case 7: return RD_C(245, 245, 250);
    case 8: return RD_C(16, 12, 24);
    case 9: return RD_C(240, 200, 60);
  }
  return 0;
}

static uint16_t rdh_capas(int x, int y) {
  // 1) Portal (drawn on top)
  { uint16_t c = rdh_vortice(x - RDH_PX, y - RDH_PY); if (c) return c; }
  // 2) Traveler
  {
    int lx = x - RDH_HERO_X, ly = y - RDH_HERO_Y;
    if ((unsigned)lx < 14 && (unsigned)ly < 18) {
      uint8_t c;
      if (rdh.pose == 2)      c = rdRectColor(RDH_HERO_C, RDH_N(RDH_HERO_C), lx, ly);
      else if (rdh.pose == 1) c = rdRectColor(RDH_HERO_B, RDH_N(RDH_HERO_B), lx, ly);
      else                    c = rdRectColor(RDH_HERO_A, RDH_N(RDH_HERO_A), lx, ly);
      if (c) return rdh_color(c);
    }
  }
  // 3) Clock
  if (y >= RDH_DIG_Y && y <= RDH_DIG_Y + 14 && x >= 35 && x <= 93) {
    for (int i = 0; i < 4; i++) {
      int lx = x - RDH_DIG_X[i], ly = y - RDH_DIG_Y;
      if ((unsigned)lx > 10) continue;
      if (rdDigito2(rdh.d[i], lx, ly)) return RD_C(240, 250, 255);
      if (rdDigito2(rdh.d[i], lx - 1, ly - 1)) return RD_C(20, 60, 150);
    }
    if (rdh.dp) {
      int lx = x - 62, ly = y - 6;
      if ((unsigned)lx < 2 && ((unsigned)ly < 2 || (unsigned)(ly - 6) < 2)) return RD_C(240, 250, 255);
      int sx = lx - 1, sy = ly - 1;
      if ((unsigned)sx < 2 && ((unsigned)sy < 2 || (unsigned)(sy - 6) < 2)) return RD_C(20, 60, 150);
    }
  }
  // 4) Platform
  if (y >= RDH_PLAT_Y) {
    if (y == RDH_PLAT_Y) return RD_C(190, 200, 225);
    if (y >= RDH_PLAT_Y + 4) return RD_C(40, 44, 64);
    return ((x & 15) == 0) ? RD_C(60, 66, 92) : RD_C(96, 106, 134);
  }
  // 5) Twinkling stars and deep sky
  unsigned v = (unsigned)x * 7919u + (unsigned)y * 104729u;
  v ^= v >> 13; v *= 0x5bd1e995u; v ^= v >> 15;
  if ((v % 97) == 0) {
    int b = (int)(((v >> 8) + (rdh.n >> 3)) & 3);
    return b == 0 ? RD_C(70, 80, 140) : b == 1 ? RD_C(140, 160, 220) : b == 2 ? RD_C(230, 240, 255) : RD_C(140, 160, 220);
  }
  return RD_C(4 + (y >> 1), 6 + y, 30 + y * 2);
}

static uint16_t rdh_pixel(int x, int y) {
  uint16_t c = rdh_capas(x, y);
  if (rdh.flash) c = rdBlanco(c, pgm_read_byte(&RDH_FLASH_K[rdh.flash > 4 ? 4 : rdh.flash]));
  return c;
}

// ────────────────────────────────────────────────────────────────────
// "MetalSlug"
// ────────────────────────────────────────────────────────────────────
#define RDE_DIG_Y     9        // top row of digits (14 pixels high -> through y=22)
#define RDE_TX        101      // tank left corner (24 pixels wide)
#define RDE_TY        18       // turret row (tracks reach y=30)
static const int16_t RDE_DIG_X[4] = { 35, 48, 69, 82 };

// Tank upper section 24x8 (turret and hull). Y body, y highlight, D shadow
static const char RDE_TANQUE[8][25] PROGMEM = {
  "........yyyyyy..........", ".......yYYYYYYy.........", ".......YYYYYYYY.........",
  "....yyyyYYYYYYYYyyyy....", "..yyYYYYYYYYYYYYYYYYyy..", ".yYYYYYYYYYYYYYYYYYYYYy.",
  ".YYYYYYYYYYYYYYYYYYYYYY.", ".DDDDDDDDDDDDDDDDDDDDDD."
};
// Plane 22x7 (facing right). R tail, C cockpit, Y/y/D fuselage, G engine
static const char RDE_AVION[7][23] PROGMEM = {
  "...RR.................", "...RRR.......CCCC.....", "....YYYYYYYYYYYYYYYY..",
  "..YYYYYYYYYYYYYYYYYYYY", "..yyYYYYYYYYYYYYYYYYYY", "..DDDDDDDDDDDDDDDDDDD.",
  "......GGGGGG...GG....."
};

struct RDEEstado {
  int8_t   d[4], obj[4];
  uint8_t  pend;            // digits not yet bombed
  uint8_t  bm[4];           // falling bomb (0 = none, 1.. = tick)
  uint8_t  st[4];           // ticks since explosion (0 = none)
  int8_t   ox[4][5], oy[4][5];
  uint8_t  rr[4][5];        // fireballs for each explosion
  bool     pon;             // plane in the sky
  int16_t  px;
  uint8_t  fire;            // tank cannon shot
  bool     jal;             // action is underway (plane or explosions): Marco jumps
  bool     dp;
  uint32_t n, ult;
  bool     ultOk;
};
static RDEEstado rde;

static void rde_marcarTanque()  { rdMarcar(RDE_TX - 12, 12, 40, 20); }
static void rde_marcarAvion()   { rdMarcar(rde.px - 3, 0, 28, 9); }
static void rde_marcarZona(int i) { rdMarcar(RDE_DIG_X[i] - 14, 6, 38, 20); }

static void rde_iniciar(int h, int m, int s) {
  rdm_digitos(h, m, rde.d);
  for (int i = 0; i < 4; i++) {
    rde.obj[i] = rde.d[i]; rde.bm[i] = 0; rde.st[i] = 0;
    for (int j = 0; j < 5; j++) { rde.ox[i][j] = 0; rde.oy[i][j] = 0; rde.rr[i][j] = 0; }
  }
  rde.pend = 0; rde.pon = false; rde.px = -22; rde.fire = 0; rde.jal = false;
  rde.dp = (s % 2 == 0); rde.n = 0; rde.ultOk = false; (void)m;
  rdMarcar(0, 0, RD_W, RD_H);
}

static void rde_cambioMinuto(int h, int m) {
  int8_t nd[4];
  rdm_digitos(h, m, nd);
  uint8_t mask = 0;
  for (int i = 0; i < 4; i++) { rde.obj[i] = nd[i]; if (nd[i] != rde.d[i]) mask |= (1 << i); }
  rde.pend = mask;                      // the tick launches the plane when there is a pending target
}

static void rde_segundo(int s) { rde.dp = (s % 2 == 0); rdMarcar(62, 13, 2, 7); }

static void rde_impacto(int i) {
  rde.bm[i] = 0; rde.st[i] = 1;
  rde.d[i] = rde.obj[i];                // old digit disappears in the explosion
  for (int j = 0; j < 5; j++) {
    uint8_t h1 = rdi_hash(i * 5 + j, 17, (int)rde.n), h2 = rdi_hash(j, i, 29);
    rde.ox[i][j] = (int)(h1 % 11) - 5;
    rde.oy[i][j] = (int)((h1 >> 4) % 9) - 4;
    rde.rr[i][j] = 4 + (h2 % 4);
  }
  rde.fire = 3;                         // the Slug fires back
  rde_marcarTanque();
}

static void rde_tick() {
  rde.n++;

  // Tank: sways, emits exhaust smoke, and fires
  if ((rde.n & 7) == 0 || (rde.n % 5) == 0 || rde.fire) rde_marcarTanque();
  if (rde.fire) rde.fire--;

  // Falling bombs and explosions
  for (int i = 0; i < 4; i++) {
    if (rde.bm[i]) {
      rde_marcarZona(i);
      if (++rde.bm[i] >= 5) rde_impacto(i);
    }
    if (rde.st[i]) {
      rde_marcarZona(i);
      if (++rde.st[i] > 30) rde.st[i] = 0;
    }
  }

  // Plane: crosses the sky and drops a bomb on each pending digit
  if (!rde.pon && rde.pend) { rde.pon = true; rde.px = -22; }
  if (rde.pon) {
    rde_marcarAvion();
    rde.px += 3;
    int cx = rde.px + 11;
    for (int i = 0; i < 4; i++)
      if ((rde.pend & (1 << i)) && cx >= RDE_DIG_X[i] + 5) {
        rde.pend &= ~(1 << i);
        rde.bm[i] = 1; rde_marcarZona(i);
      }
    if (rde.px > 130) rde.pon = false;
    else rde_marcarAvion();
  }

  // Marco jumps while a plane or explosions are present; redraw the tank on each action tick
  bool jal = rde.pon || rde.st[0] || rde.st[1] || rde.st[2] || rde.st[3];
  if (jal || jal != rde.jal) rde_marcarTanque();
  rde.jal = jal;
}

static void rde_paso(uint32_t ms) {
  if (!rde.ultOk) { rde.ult = ms; rde.ultOk = true; return; }
  int n = 0;
  while ((uint32_t)(ms - rde.ult) >= RDM_TICK_MS && n < 3) { rde.ult += RDM_TICK_MS; rde_tick(); n++; }
  if (n == 3) rde.ult = ms;
}

// The digit is visible whole, hidden during the explosion, and flashes when it returns
static bool rde_visible(int i) {
  int s = rde.st[i];
  if (s == 0 || s >= 24) return true;
  if (s < 12) return false;
  return (((s >> 1) & 1) == 0);
}
static bool rde_lit(int i, int lx, int ly) {
  if ((unsigned)lx >= 10 || (unsigned)ly >= 14 || !rde_visible(i)) return false;
  return (pgm_read_byte(&RDM_FUENTE[rde.d[i]][ly >> 1]) & (0x10 >> (lx >> 1))) != 0;
}

static uint16_t rde_pixel(int x, int y) {
  // 1) Explosions: fireballs (white -> yellow -> orange -> red -> gray smoke)
  for (int i = 0; i < 4; i++) {
    int s = rde.st[i];
    if (!s || s > 20) continue;
    if (x < RDE_DIG_X[i] - 14 || x > RDE_DIG_X[i] + 24) continue;
    for (int j = 0; j < 5; j++) {
      int tj = s - j / 2;
      if (tj < 1) continue;
      int R = rde.rr[i][j];
      int r = (tj <= 4) ? tj * R / 4 : (tj <= 8 ? R : R - (tj - 8));
      if (r <= 0) continue;
      int dx = x - (RDE_DIG_X[i] + 5 + rde.ox[i][j]), dy = y - (RDE_DIG_Y + 7 + rde.oy[i][j]);
      int d2 = dx * dx + dy * dy;
      if (d2 > r * r) continue;
      bool nucleo = d2 <= (r * r) / 3;
      switch (tj >> 2) {
        case 0:  return nucleo ? RD_C(255, 255, 230) : RD_C(255, 220, 60);
        case 1:  return nucleo ? RD_C(255, 230, 80)  : RD_C(255, 130, 20);
        case 2:  return nucleo ? RD_C(255, 150, 30)  : RD_C(200, 50, 20);
        case 3:  return nucleo ? RD_C(120, 70, 60)   : RD_C(80, 60, 60);
        default: return nucleo ? RD_C(110, 105, 108) : RD_C(75, 72, 78);
      }
    }
  }
  // 2) Falling bombs
  for (int i = 0; i < 4; i++) {
    if (!rde.bm[i]) continue;
    int dx = x - (RDE_DIG_X[i] + 5), dy = y - (6 + 2 * rde.bm[i]);
    if (dy == -2 && abs(dx) == 1) return RD_C(150, 120, 40);
    if (abs(dx) <= 1 && dy >= -1 && dy <= 2) return (dx == -1 && dy == 0) ? RD_C(130, 130, 150) : RD_C(40, 40, 52);
  }
  // 3) Plane
  if (rde.pon) {
    int lx = x - rde.px, ly = y - 1;
    if ((unsigned)lx < 22 && (unsigned)ly < 7) {
      char ch = (char)pgm_read_byte(&RDE_AVION[ly][lx]);
      switch (ch) {
        case 'R': return RD_C(210, 50, 40);
        case 'C': return RD_C(110, 200, 255);
        case 'Y': return RD_C(214, 190, 70);
        case 'y': return RD_C(252, 236, 140);
        case 'D': return RD_C(150, 120, 40);
        case 'G': return RD_C(110, 110, 125);
      }
    }
  }
  // 4) Tank with Marco, cannon, tracks, and exhaust smoke
  if (x >= RDE_TX - 12 && x <= 127) {
    int bob = (rde.n >> 3) & 1;
    int u = x - RDE_TX, v = y - (RDE_TY + bob);
    // cannon muzzle flash
    if (rde.fire) {
      int dx = x - (RDE_TX + 24), dy = y - (RDE_TY + bob);
      if (dx * dx + dy * dy <= 9) return (dx * dx + dy * dy <= 3) ? RD_C(255, 255, 230) : RD_C(255, 190, 50);
    }
    // cannon (recoils when fired)
    int rc = rde.fire ? 2 : 0;
    if (u >= 15 && u <= 22 - rc) {
      int top = (u <= 18) ? 1 : 0;
      if (v == top || v == top + 1) {
        if (u == 22 - rc) return RD_C(60, 50, 20);
        return (v == top) ? RD_C(200, 180, 70) : RD_C(120, 100, 30);
      }
    }
    // Marco leaning out (jumps during action)
    int anima = (rde.pon || rde.st[0] || rde.st[1] || rde.st[2] || rde.st[3]) ? ((rde.n >> 1) & 1) : 0;
    int vh = v + anima;
    if (u >= 8 && u <= 12) {
      if (vh == -4 && u >= 9 && u <= 11) return RD_C(110, 70, 40);
      if (vh == -3) return RD_C(220, 40, 40);
      if (vh == -2) return (u == 10) ? RD_C(20, 20, 20) : RD_C(252, 200, 160);
      if (vh == -1 && u >= 9 && u <= 11) return RD_C(252, 200, 160);
    }
    // hull
    if ((unsigned)u < 24 && (unsigned)v < 8) {
      char ch = (char)pgm_read_byte(&RDE_TANQUE[v][u]);
      if (ch == 'Y') return RD_C(214, 190, 70);
      if (ch == 'y') return RD_C(252, 236, 140);
      if (ch == 'D') return RD_C(150, 120, 40);
    }
    // tracks and wheels
    if (v >= 8 && v <= 12) {
      bool oruga = (v == 8 || v == 12) ? (u >= 3 && u <= 20) : (u >= 1 && u <= 22);
      if (oruga) {
        for (int k = 0; k < 4; k++) {
          int cu = 4 + k * 5, du = u - cu, dv = v - 10;
          if (du * du + dv * dv <= 4) return (du == 0 && dv == 0) ? RD_C(40, 40, 50) : RD_C(130, 130, 142);
        }
        return RD_C(24, 24, 30);
      }
    }
    // exhaust smoke, behind the tank
    {
      int f = (rde.n / 5) % 3;
      int dx = x - (RDE_TX - 2 - 2 * f), dy = y - (RDE_TY + bob + 5 - 2 * f);
      if (dx * dx + dy * dy <= 1 + f) return RD_C(170, 165, 160);
    }
  }
  // 5) Scoreboard-style digits: yellow-orange gradient with dark outline
  for (int i = 0; i < 4; i++) {
    int lx = x - RDE_DIG_X[i];
    if (lx < -1 || lx > 10) continue;
    int ly = y - RDE_DIG_Y;
    if (ly < -1 || ly > 14) break;
    if (rde_lit(i, lx, ly)) return (ly < 4) ? RD_C(255, 246, 130) : (ly < 9) ? RD_C(255, 208, 60) : RD_C(255, 150, 30);
    for (int dy = -1; dy <= 1; dy++)
      for (int dx = -1; dx <= 1; dx++)
        if ((dx || dy) && rde_lit(i, lx + dx, ly + dy)) return RD_C(80, 28, 10);
    break;
  }
  if (rde.dp && (unsigned)(x - 62) < 2 && ((unsigned)(y - 13) < 2 || (unsigned)(y - 18) < 2))
    return RD_C(255, 220, 80);

  // 6) Ruins silhouetted against the sky
  if (y <= 23) {
    int h = 4 + (rdi_hash(x / 9, 2, 1) % 10);
    if (y >= 24 - h) return RD_C(58, 30, 30);
    return RD_C(28 + y * 9, 18 + y * 5, 70 - y * 2);               // sunset sky
  }
  // 7) Sand
  if (y == 24) return RD_C(222, 184, 112);
  if (y == 30) return RD_C(160, 120, 60);
  return (rdi_hash(x, y, 5) < 40) ? RD_C(170, 128, 66) : RD_C(196, 152, 86);
}

#endif // RD_16BIT_H
