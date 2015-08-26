#ifndef PLAYER_H
#define PLAYER_H

#include "spriteLib.h"
#include "types.h"

typedef enum {ANIM_STANDING, ANIM_WALKING, ANIM_JUMPING} PlayerAnimState;

int animationFrames[] = 
             {4,             2,            4           };
typedef enum {FACING_RIGHT, FACING_LEFT} PlayerDirection;

typedef struct
{
    int row;
    int col;
    int width;
    int height;
    int verticalSpeed;
    int horizontalSpeed;
    int health;
    int dimension;
    Sprite* sprite;
    bool canJump;
    PlayerAnimState animationState;
    PlayerAnimState previousAnimationState;
    PlayerDirection direction;
    int animationCounter;
    int maximumShiftCharge;
    int currentShiftCharge;
} Player;

#endif
