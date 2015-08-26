#include "spriteLib.h"

void updateAllSpriteAttributes(int spriteNumber, int attr0, int attr1, int attr2)
{
    shadowOAM[spriteNumber].attribute0 = attr0;
    shadowOAM[spriteNumber].attribute1 = attr1;
    shadowOAM[spriteNumber].attribute2 = attr2;
}

void updateSprite(int spriteNumber, int row, int col, int spriteOffsetRow, int spriteOffsetCol, int horizontalFlip)
{
    shadowOAM[spriteNumber].attribute0 = (shadowOAM[spriteNumber].attribute0 & ~ROWMASK) | (row & ROWMASK);
    shadowOAM[spriteNumber].attribute1 = (shadowOAM[spriteNumber].attribute1 & ~COLMASK) | (col & COLMASK);
    if (horizontalFlip)
        shadowOAM[spriteNumber].attribute1 |= ATTR1_HFLIP;
    else
        shadowOAM[spriteNumber].attribute1 &= ~ATTR1_HFLIP;
    shadowOAM[spriteNumber].attribute2 = (shadowOAM[spriteNumber].attribute2 & 0xFC00) | SPRITEOFFSET(spriteOffsetRow, spriteOffsetCol);
}
