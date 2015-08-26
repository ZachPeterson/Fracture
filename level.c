#include "level.h"
#include "stdlib.h"


Level* loadLevel(unsigned short dim0BackTileLen, unsigned short* dim0BackTiles, unsigned short dim0BackMapLen, unsigned short* dim0BackMap, unsigned short* dim0BackPal, unsigned short dim0ColMapLen, unsigned short* dim0ColMapData, unsigned short dim1BackTileLen, unsigned short* dim1BackTiles, unsigned short dim1BackMapLen, unsigned short* dim1BackMap, unsigned short* dim1BackPal, unsigned short dim1ColMapLen, unsigned short* dim1ColMapData)
    {
    Level* tempLevel = malloc(sizeof(Level));
    if (tempLevel != NULL)
    {
        tempLevel->dimension = malloc(sizeof(Dimension) * 2);
        if (tempLevel->dimension != NULL)
        {
            Dimension* dim0 = malloc(sizeof(Dimension));
            Dimension* dim1 = malloc(sizeof(Dimension));
            if (dim0 != NULL && dim1 != NULL)
            {
                LevelBackground* dim0Back = malloc(sizeof(LevelBackground));
                LevelBackground* dim1Back = malloc(sizeof(LevelBackground));
                if (dim0Back != NULL && dim1Back != NULL)
                {
                    CollisionMap* dim0ColMap = malloc(sizeof(CollisionMap));
                    CollisionMap* dim1ColMap = malloc(sizeof(CollisionMap));
                    if (dim0ColMap != NULL && dim1ColMap != NULL)
                    {
                        dim0Back->backgroundTilesLength = dim0BackTileLen;
                        dim0Back->backgroundTiles = dim0BackTiles;
                        dim0Back->backgroundMapLength = dim0BackMapLen;
                        dim0Back->backgroundMap = dim0BackMap;
                        dim0Back->backgroundPalette = dim0BackPal;

                        dim1Back->backgroundTilesLength = dim1BackTileLen;
                        dim1Back->backgroundTiles = dim1BackTiles;
                        dim1Back->backgroundMapLength = dim1BackMapLen;
                        dim1Back->backgroundMap = dim1BackMap;
                        dim1Back->backgroundPalette = dim1BackPal;

                        dim0ColMap->collisionMapLength = dim0ColMapLen;
                        dim0ColMap->collisionMapWidth = dim0ColMapLen / 8;
                        dim0ColMap->collisionMapHeight = 256;
                        dim0ColMap->collisionMapData = dim0ColMapData;

                        dim1ColMap->collisionMapLength = dim1ColMapLen;
                        dim1ColMap->collisionMapWidth = dim1ColMapLen / 8;
                        dim1ColMap->collisionMapHeight = 256;
                        dim1ColMap->collisionMapData = dim1ColMapData;

                        dim0->dimensionBackground = dim0Back;
                        dim0->dimensionCollisionMap = dim0ColMap;

                        dim1->dimensionBackground = dim1Back;
                        dim1->dimensionCollisionMap = dim1ColMap;

                        tempLevel->dimension[0] = dim0;
                        tempLevel->dimension[1] = dim1;

                        return tempLevel;
                    }
                }
            }
        }
    }

    return NULL;
}
