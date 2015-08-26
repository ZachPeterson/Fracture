#ifndef SPRITELIB_H
#define SPRITELIB_H

#define ROWMASK (0xFF)
#define COLMASK (0x1FF)

#define SPRITEOFFSET(r, c) ((r) * 32 + (c))

typedef struct
{
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
} OAMAttributes;

#define SPRITEMEMORY ((OAMAttributes*)0x7000000)
#define SPRITEDATA ((unsigned short*)0x6010000)
#define SPRITEPALETTE ((unsigned short*)0x5000200)
#define OBJ_ENABLE (1 << 12)
#define MODE1D (1 << 6)

// Attribute 0 #defines
#define ATTR0_REG     (0 << 8)
#define ATTR0_AFF     (1 << 8)
#define ATTR0_HIDE    (2 << 8)
#define ATTR0_AFF_DBL (3 << 8)
#define ATTR0_BLEND   (1 << 10)
#define ATTR0_WIN     (2 << 10)
#define ATTR0_MOSAIC  (1 << 12)
#define ATTR0_4BPP    (0 << 13)
#define ATTR0_8BPP    (1 << 13)
#define ATTR0_SQUARE  (0 << 14)
#define ATTR0_WIDE    (1 << 14)
#define ATTR0_TALL    (2 << 14)

// Attribute 1 #defines
#define ATTR1_NOFLIP (0 << 12)
#define ATTR1_HFLIP  (1 << 12)
#define ATTR1_VFLIP  (1 << 13)

//                                  col x row
// *------------------------------------------------------------
// *   Size       Bits        00      01      10      11
// *
// *   Shape       00         8x8     16x16   32x32   64x64
// *   Wide        01         16x8    32x8    32x16   64x32
// *   Tall        10         8x16    8x32    16x32   32x64
// *------------------------------------------------------------
#define ATTR1_SIZE8  (0 << 14)
#define ATTR1_SIZE16 (1 << 14)
#define ATTR1_SIZE32 (2 << 14)
#define ATTR1_SIZE64 (3 << 14)

// Attribute 2 #defines
#define ATTR2_PRIORITY0 (0 << 10)
#define ATTR2_PRIORITY1 (1 << 10)
#define ATTR2_PRIORITY2 (2 << 10)
#define ATTR2_PRIORITY3 (3 << 10)

#define ATTR2_PALETTE_BANK(bank) ((bank) << 12)

OAMAttributes shadowOAM[128];

typedef struct
{
    int row;
    int col;
    int start;
    int shape;
    int size;
    int width;
    int height;
} Sprite;

void updateAllSpriteAttributes(int spriteNumber, int attr0, int attr1, int attr2);
void updateSprite(int spriteNumber, int row, int col, int spriteOffsetRow, int spriteOffsetCol, int horizontalFlip);

#endif
