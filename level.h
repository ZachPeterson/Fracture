#ifndef LEVEL_H
#define LEVEL_H

typedef struct
{
    unsigned short backgroundTilesLength;
    const unsigned short* backgroundTiles;
    unsigned short backgroundMapLength;
    const unsigned short* backgroundMap;
    const unsigned short* backgroundPalette;
} LevelBackground;

typedef struct
{
    unsigned short collisionMapLength;
    unsigned short collisionMapWidth;
    unsigned short collisionMapHeight;
    const unsigned short* collisionMapData;
} CollisionMap;

typedef struct
{
    LevelBackground* dimensionBackground;
    CollisionMap* dimensionCollisionMap;
} Dimension;

typedef struct level
{
    Dimension** dimension;
    struct level* nextLevel;
    int dimensionGoalDistance;
    int winningDimension;
    int startX;
    int startY;
} Level;

Level* loadLevel(unsigned short dim0BackTileLen, unsigned short* dim0BackTiles, unsigned short dim0BackMapLen, unsigned short* dim0BackMap, unsigned short* dim0BackPal, unsigned short dim0ColMapLen, unsigned short* dim0ColMapData, unsigned short dim1BackTileLen, unsigned short* dim1BackTiles, unsigned short dim1BackMapLen, unsigned short* dim1BackMap, unsigned short* dim1BackPal, unsigned short dim1ColMapLen, unsigned short* dim1ColMapData);

#endif
