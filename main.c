// **************************************************
// For my game, I am making a game similar to Mario
// or Super Meat Boy. It will be a platformer with
// dangers to avoid and maybe enemies to kill if time
// permits. The twist is that part of the level will
// exist in an alternate dimension and the player
// will have to switch between dimensions to complete
// the level.
// **************************************************
// Include standard library header files
#include <stdio.h>
#include <stdlib.h>

// Include project header files
#include "dmaLib.h"
#include "buttonLib.h"
#include "interruptLib.h"
#include "level.h"
#include "myLib.h"
#include "player.h"
#include "soundLib.h"
#include "spriteLib.h"
#include "text.h"
#include "timerLib.h"
#include "types.h"
#include "videoLib.h"

#include "./assets/music/entangledSong.h"
#include "./assets/music/gameSong.h"
#include "./assets/music/loseSong.h"
#include "./assets/music/menuSong.h"
#include "./assets/music/winSong.h"

#include "./assets/sound/jump.h"
#include "./assets/sound/shift.h"

#include "./assets/art/entangledScreen.h"
#include "./assets/art/logo.h"
#include "./assets/art/menu.h"
#include "./assets/art/instructions.h"
#include "./assets/art/winScreen.h"
#include "./assets/art/loseScreen.h"
#include "./assets/art/pauseScreen.h"

#include "./assets/art/spriteSheet.h"

#include "./assets/art/smokeBackground.h"

#include "./assets/art/level1Dimension0.h"
#include "./assets/art/level1Dimension0CollisionMap.h"
#include "./assets/art/level1Dimension1.h"
#include "./assets/art/level1Dimension1CollisionMap.h"

#include "./assets/art/level2Dimension0.h"
#include "./assets/art/level2Dimension0CollisionMap.h"
#include "./assets/art/level2Dimension1.h"
#include "./assets/art/level2Dimension1CollisionMap.h"

#define SHIFTUP(i) ((i) << 8)
#define SHIFTDOWN(i) ((i) >> 8)

unsigned long vbCount = 0;

typedef enum {STATE_LOGO, STATE_MENU, STATE_INSTRUCTIONS, STATE_GAME, STATE_PAUSE, STATE_WIN, STATE_LOSE, STATE_ENTANGLED} states;
states currentState = STATE_LOGO;

void setupLogo();
void logo();
void setupMenu();
void menu();
void setupInstructions();
void instructions();
void setupGame();
void setupGameBackgrounds();
void setupSmokeBackground();
void setupGameBackgroundPalette();
void setupSprites();
void game();
void updatePlayerMovement();
void setupPause();
void pause();
void setupWin();
void win();
void setupLose();
void lose();
void setupEntangled();
void entangled();

void setupMode0();
void setupMode4();

int currentInstruction = 0;

int hoff = 0;
int voff = 0;

int smokeHOff = 0;
int smokeVOff = 0;

void backgroundHandler();

int backgroundIndex = 0;
int maxBackgrounds = 0;

unsigned long logoVBCount = 0;

Sprite playerSprite = {32, 32, SPRITEOFFSET(0, 0), ATTR0_TALL, ATTR1_SIZE32, 16, 32};

Level* currentLevel;
Level* level1;
Level* level2;

int gravity = 35;
// Small gravity for falling collision testing
// int gravity = 5;

Player player = {SHIFTUP(32), SHIFTUP(32), 16, 32, 0, 1, 100, 0, &playerSprite, FALSE, ANIM_STANDING, ANIM_STANDING, FACING_RIGHT, 0, 90, 90};

int main()
{
    setupInterrupts();
    setupSound();

    level1 = loadLevel(level1Dimension0TilesLen, level1Dimension0Tiles, level1Dimension0MapLen, level1Dimension0Map, level1Dimension0Pal, level1Dimension0CollisionMapBitmapLen, level1Dimension0CollisionMapBitmap, level1Dimension1TilesLen, level1Dimension1Tiles, level1Dimension1MapLen, level1Dimension1Map, level1Dimension1Pal, level1Dimension1CollisionMapBitmapLen, level1Dimension1CollisionMapBitmap);
    level2 = loadLevel(level2Dimension0TilesLen, level2Dimension0Tiles, level2Dimension0MapLen, level2Dimension0Map, level2Dimension0Pal, level2Dimension0CollisionMapBitmapLen, level2Dimension0CollisionMapBitmap, level2Dimension1TilesLen, level2Dimension1Tiles, level2Dimension1MapLen, level2Dimension1Map, level2Dimension1Pal, level2Dimension1CollisionMapBitmapLen, level2Dimension1CollisionMapBitmap);

    level1->nextLevel = level2;
    level1->dimensionGoalDistance = 1858;
    level1->winningDimension = 0;
    level1->startX = 15;
    level1->startY = 64;

    level2->nextLevel = NULL;
    level2->dimensionGoalDistance = 4002;
    level2->winningDimension = 1;
    level2->startX = 5;
    level2->startY = 192;

    currentLevel = level1;

    setupMode4();

    setupLogo();
    
    while(TRUE)
    {
        pollButtons();

        switch (currentState)
        {
            case STATE_LOGO:
                logo();
                break;
            case STATE_MENU:
                menu();
                break;
            case STATE_INSTRUCTIONS:
                instructions();
                break;
            case STATE_GAME:
                game();
                break;
            case STATE_PAUSE:
                pause();
                break;
            case STATE_WIN:
                win();
                break;
            case STATE_LOSE:
                lose();
                break;
            case STATE_ENTANGLED:
                entangled();
                break;
            default:
                exit(-1);
                break;
        }
    }

    return 1;
}

void gameInterruptHandler()
{
    REG_IME = 0;
    
    if (REG_IF & INT_VB)
    {
        vbCount++;

        soundInterruptHandler();

        backgroundHandler();
        
        REG_IF = INT_VB;
    }

    REG_IME = 1;
}

void backgroundHandler()
{
    // Check to see if the screen to the right should be loaded
    if (hoff > 268)
    {
        // Check to see if another background can be drawn
        if (backgroundIndex + 2 < maxBackgrounds)
        {
            backgroundIndex++;
            hoff -= 256;

            setupGameBackgrounds();
        }
    }
    // Check to see if the screen to the left should loaded
    else if (hoff < 4)
    {
        // Check to see if another background can be drawn
        if (backgroundIndex > 0)
        {
            backgroundIndex--;
            hoff += 256;
            
            setupGameBackgrounds();
        }
    }

    if (hoff < 0)
    {
        hoff = 0;
    }
    else if (hoff > 272)
    {
        hoff = 272;
    }

    REG_BG0HOFS = hoff;

    if (voff < 0)
    {
        voff = 0;
    }
    else if (voff > 96)
    {
        voff = 96;
    }
    REG_BG0VOFS = voff;
}

void setupLogo()
{
    DMANow(3, logoPal, PALETTE, 256);

    logoVBCount = 0;
}

void logo()
{
    logoVBCount++;

    drawBackgroundImage4(logoBitmap);

    flipPage();

    waitForVBlank();

    if (logoVBCount > 120 || isButtonHit(BUTTON_START))
    {
        currentState = STATE_MENU;

        fillScreen4(0);

        playSoundA(menuSong, MENUSONGLEN, MENUSONGFREQ, TRUE);

        setupMenu();
    }
}

void setupMenu()
{
    loadPalette(menuPal);
    player.maximumShiftCharge = 90;
}

void menu()
{
    drawBackgroundImage4(menuBitmap);

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_GAME;

        fillScreen4(0);

        currentLevel = level1;

        playSoundA(gameSong, GAMESONGLEN, GAMESONGFREQ, TRUE);

        setupGame();
    }

    if (isButtonHit(BUTTON_SELECT))
    {
        currentState = STATE_INSTRUCTIONS;

        fillScreen4(0);
        setupInstructions();
    }

    if (isButtonHit(BUTTON_R))
    {
        player.maximumShiftCharge = 1000;
    }
}

void setupInstructions()
{
    loadPalette(instructionsPal);
    currentInstruction = 0;
}

void instructions()
{
    DMANow(3, instructionsBitmap + ((240 * 160) / 2) * currentInstruction, videoBuffer, ((240 * 160)/2));

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_A))
    {
        if (currentInstruction < 6)
        {
            currentInstruction++;
        }
    }

    if (isButtonHit(BUTTON_START) && currentInstruction == 6)
    {
        currentState = STATE_GAME;

        fillScreen4(0);

        currentLevel = level1;

        playSoundA(gameSong, GAMESONGLEN, GAMESONGFREQ, TRUE);

        setupGame();
    }

    if (isButtonHit(BUTTON_SELECT) && currentInstruction == 6)
    {
        currentState = STATE_MENU;

        fillScreen4(0);

        setupMenu();
    }
}

void setupGame()
{
    maxBackgrounds = currentLevel->dimension[0]->dimensionBackground->backgroundMapLength / 2048;

    setupMode0();

    REG_BG0VOFS = 0;
    REG_BG0HOFS = 0;
    player.row = SHIFTUP(currentLevel->startY);
    player.col = SHIFTUP(currentLevel->startX);
    player.verticalSpeed = 0;
    player.horizontalSpeed = 1;
    player.canJump = FALSE;
    player.health = 100;
    player.dimension = 0;
    (player.sprite)->row = SHIFTDOWN(player.row);
    (player.sprite)->col = SHIFTDOWN(player.col);
    player.animationCounter = 0;
    player.animationState = ANIM_STANDING;
    player.previousAnimationState = ANIM_STANDING;
    player.direction = FACING_RIGHT;
    player.currentShiftCharge = player.maximumShiftCharge;


    unmuteSound();

    voff = 0;
    hoff = 0;

    backgroundIndex = 0;

    setupGameBackgrounds();
    setupSmokeBackground();
    setupGameBackgroundPalette();
    setupSprites();

    shadowOAM[0].attribute0 = (player.sprite)->row | ATTR0_4BPP | (player.sprite)->shape;
    shadowOAM[0].attribute1 = (player.sprite)->col | (player.sprite)->size;
    shadowOAM[0].attribute2 = (player.sprite)->start | ATTR2_PALETTE_BANK(0) | 2 << 10;

    shadowOAM[1].attribute0 = 140 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[1].attribute1 = 45 | ATTR1_SIZE32;
    shadowOAM[1].attribute2 = SPRITEOFFSET(0, 24) | ATTR2_PALETTE_BANK(0);

    shadowOAM[2].attribute0 = 140 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[2].attribute1 = 5 | ATTR1_SIZE32;
    shadowOAM[2].attribute2 = SPRITEOFFSET(0, 24) | ATTR2_PALETTE_BANK(1);

    shadowOAM[3].attribute0 = 140 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[3].attribute1 = 5 | ATTR1_SIZE32;
    shadowOAM[3].attribute2 = SPRITEOFFSET(28, 20) | ATTR2_PALETTE_BANK(0);

    shadowOAM[4].attribute0 = 140 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[4].attribute1 = 45 | ATTR1_SIZE32;
    shadowOAM[4].attribute2 = SPRITEOFFSET(30, 20) | ATTR2_PALETTE_BANK(0);

    int i;
    for (i = 5; i < 128; i++)
    {
        shadowOAM[i].attribute0 = ATTR0_HIDE;
    }

    DMANow(3, shadowOAM, SPRITEMEMORY, (128 * sizeof(OAMAttributes) / 2));

}

void setupSprites()
{
    DMANow(3, spriteSheetPal, SPRITEPALETTE, 256);
    DMANow(3, spriteSheetTiles, CHARACTERBLOCKBASE + 4, 32768 / 2);
}

void setupGameBackgrounds()
{
    loadTiles(currentLevel->dimension[player.dimension]->dimensionBackground->backgroundTiles, 0, currentLevel->dimension[player.dimension]->dimensionBackground->backgroundTilesLength);
    DMANow(3, currentLevel->dimension[player.dimension]->dimensionBackground->backgroundMap + (1024 * backgroundIndex), SCREENBLOCKBASE + 30, 2048);
}

void setupSmokeBackground()
{
    loadTiles(smokeBackgroundTiles, 2, smokeBackgroundTilesLen);
    DMANow(3, smokeBackgroundMap, SCREENBLOCKBASE + 28, 2048);
}

void setupGameBackgroundPalette()
{
    loadPalette(currentLevel->dimension[player.dimension]->dimensionBackground->backgroundPalette);
}

void game()
{
    int newDimension = -1;
    player.previousAnimationState = player.animationState;
    player.animationState = ANIM_STANDING;

    if (SHIFTDOWN(player.col) > currentLevel->dimensionGoalDistance && player.dimension == currentLevel->winningDimension)
    {
        if (currentLevel->nextLevel != NULL)
        {
            currentLevel = currentLevel->nextLevel;
            setupGame();
            return;
        }
        else
        {
            currentState = STATE_WIN;

            setupWin();

            playSoundA(winSong, WINSONGLEN, WINSONGFREQ, TRUE);

            return;
        }
    }
    if (SHIFTDOWN(player.row) > 256 - player.height - SHIFTDOWN(player.verticalSpeed))
    {
        currentState = STATE_LOSE;

        setupLose();

        playSoundA(loseSong, LOSESONGLEN, LOSESONGFREQ, TRUE);

        return;
    }

    if (player.health <= 0)
    {
        currentState = STATE_ENTANGLED;

        setupEntangled();

        playSoundA(entangledSong, ENTANGLEDSONGLEN, ENTANGLEDSONGFREQ, TRUE);

        return;
    }

    if (player.dimension)
    {
        player.currentShiftCharge--;
    }

    if (player.currentShiftCharge < player.maximumShiftCharge && !player.dimension && vbCount % 3 == 0)
    {
        player.currentShiftCharge++;
    }

    if (player.dimension && player.currentShiftCharge <= 0)
    {
        newDimension = 0;
    }
    
    // Update player movement
    updatePlayerMovement();

    if (!player.canJump)
    {
        player.animationState = ANIM_JUMPING;
    }


    if (isButtonHit(BUTTON_B))
    {
        if (player.dimension)
        {
            newDimension = 0;
        }
        else
        {
            if (player.currentShiftCharge > 0)
            {
                newDimension = 1;
            }
        }
    }

    if (player.dimension)
    {
        player.currentShiftCharge--;
    }

    if (vbCount % 15 == 0)
    {
        player.animationCounter++;
    }

    if (player.animationCounter >= animationFrames[player.animationState])
    {
        player.animationCounter = 0;
    }

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_PAUSE;

        setupPause();
    }

    

    int horizontalScreenBounds = 80;
    int verticalScreenBounds = 30;

    if (SHIFTDOWN(player.col) > (hoff + 256 * backgroundIndex + (240 - horizontalScreenBounds - (player.sprite)->width)))
    {
        hoff = SHIFTDOWN(player.col) - (256 * backgroundIndex + 240 - horizontalScreenBounds - (player.sprite)->width);
    }
    else if (SHIFTDOWN(player.col) < (hoff + 256 * backgroundIndex + horizontalScreenBounds))
    {
        hoff = SHIFTDOWN(player.col) - (256 * backgroundIndex + horizontalScreenBounds);
    }
    if (SHIFTDOWN(player.row) < voff + verticalScreenBounds)
    {
        voff = SHIFTDOWN(player.row) - verticalScreenBounds;
    }
    else if (SHIFTDOWN(player.row) + (player.sprite)->height > voff + (160 - verticalScreenBounds))
    {
        voff = SHIFTDOWN(player.row) + (player.sprite)->height - (160 - verticalScreenBounds);
    }



    int shiftRatio = 0;
    shiftRatio = (player.currentShiftCharge * 30) / player.maximumShiftCharge;
    int shiftRow = 30 - shiftRatio;
    if (shiftRow < 0)
        shiftRow = 0;
    int shiftCol = 0;
    if (shiftRow > 15)
    {
        shiftRow -= 15;
        shiftCol = 1;
    }
    if (shiftRow > 15)
    {
        shiftRow = 15;
    }

    int healthRatio = 0;
    healthRatio = (player.health * 30) / 100;
    int healthRow = 30 - healthRatio;
    if (healthRow < 0)
        healthRow = 0;
    int healthCol = 0;
    if (healthRow > 15)
    {
        healthRow -= 15;
        healthCol = 1;
    }
    if (healthRow > 15)
    {
        healthRow = 15;
    }

    shadowOAM[1].attribute2 = SPRITEOFFSET(shiftRow * 2, 24 + (4 * shiftCol)) | ATTR2_PALETTE_BANK(0);
    shadowOAM[2].attribute2 = SPRITEOFFSET(healthRow * 2, 24 + (4 * healthCol)) | ATTR2_PALETTE_BANK(1);


    waitForVBlank();


    if (vbCount % 3 == 0)
    {
        smokeVOff += 2;
        smokeHOff -= 1;
        if (smokeVOff > 512)
        {
            smokeVOff -= 512;
        }
        if (smokeHOff < 0)
        {
            smokeHOff += 256;
        }

        REG_BG1VOFS = smokeVOff + voff;
        REG_BG1HOFS = smokeHOff + hoff;
    }

    if (newDimension != -1)
    {
        player.dimension = newDimension;
        setupGameBackgrounds();
        setupGameBackgroundPalette();
        playSoundB(shift, SHIFTLEN, SHIFTFREQ, FALSE);
    }

    (player.sprite)->row = SHIFTDOWN(player.row) - voff;
    (player.sprite)->col = SHIFTDOWN(player.col) - (hoff + 256 * backgroundIndex);

    int shiftAnimOffset = 0;
    if (player.dimension)
    {
        shiftAnimOffset = 12;
    }

    updateSprite(0, (player.sprite)->row, (player.sprite)->col, 4 * player.animationState + shiftAnimOffset, player.animationCounter * 2, player.direction);

    DMANow(3, shadowOAM, SPRITEMEMORY, (128 * sizeof(OAMAttributes) / 2));
}

void updatePlayerMovement()
{
    player.verticalSpeed += gravity;

    int i;
    bool entangled = FALSE;
    // Check for object entanglement
    // Check the top of the player
    if (!entangled)
    {
        for (i = 1; i < player.width - 1; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + 1, SHIFTDOWN(player.col) + i, 256 * maxBackgrounds)] == 0)
            {
                entangled = TRUE;
                break;
            }
        }
    }
    // Check the bottom of the player
    if (!entangled)
    {
        for (i = 1; i < player.width - 1; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + player.height - 1, SHIFTDOWN(player.col) + i, 256 * maxBackgrounds)] == 0)
            {
                entangled = TRUE;
                break;
            }
        }
    }
    if (!entangled)
    {
        for (i = 1; i < player.height - 1; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + i, SHIFTDOWN(player.col) + 1, 256 * maxBackgrounds)] == 0)
            {
                entangled = TRUE;
                break;
            }
        }
    }
    if (!entangled)
    {
        for (i = 1; i < player.height - 1; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + i, SHIFTDOWN(player.col) +player.width - 1, 256 * maxBackgrounds)] == 0)
            {
                entangled = TRUE;
                break;
            }
        }
    }

    // Take away health if the player is entangled
    if (entangled)
    {
        player.health -= 3;
    }

    if (isButtonDown(BUTTON_A))
    {
        if (player.canJump)
        {
            player.verticalSpeed = -1000;
            player.canJump = FALSE;
            playSoundB(jump, JUMPLEN, JUMPFREQ, FALSE);
        }
    }

    if (player.verticalSpeed < 0)
    {
        int collided = 0;
        for (i = 0; i < player.width; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + SHIFTDOWN(player.verticalSpeed), SHIFTDOWN(player.col) + i, 256 * maxBackgrounds)] == 0)
            {
                collided = 1;
                break;
            }
        }
        if (!collided)
        {
            player.row += player.verticalSpeed;
        }
        else
        {
            player.verticalSpeed = 0;
        }
    }
    else
    {
        int collided = 0;
        for (i = 0; i < player.width; i++)
        {
            if (SHIFTDOWN(player.row) + SHIFTDOWN(player.verticalSpeed) + player.height + 1 >= 256)
            {
                currentState = STATE_LOSE;

                setupLose();

                playSoundA(loseSong, LOSESONGLEN, LOSESONGFREQ, TRUE);

                return;
            }
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + SHIFTDOWN(player.verticalSpeed) + player.height + 1, SHIFTDOWN(player.col) + i, 256 * maxBackgrounds)] == 0)
            {
                collided = 1;
                break;
            }
        }
        if (!collided)
        {
            player.row += player.verticalSpeed;
            player.canJump = FALSE;
        }
        else
        {
            player.verticalSpeed = 0;
            player.canJump = TRUE;
            int j;
            bool collisionAtLevel = TRUE;
            for (j = SHIFTDOWN(player.verticalSpeed); j >= 0; j--)
            {
                collisionAtLevel = FALSE;
                for (i = 0; i < player.width; i++)
                {
                    if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + j + player.height, SHIFTDOWN(player.col) + i, 256 * maxBackgrounds)] == 0)
                    {
                        collisionAtLevel = TRUE;
                    }
                }
                if (!collisionAtLevel)
                {
                    player.row += SHIFTUP(j + 1);
                    break;
                }
            }
        }
    }
    
    if (isButtonDown(BUTTON_LEFT))
    {
        int collided = 0;
        for (i = 0; i < player.height; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + i, SHIFTDOWN(player.col) - player.horizontalSpeed, 256 * maxBackgrounds)] == 0)
            {
                collided = 1;
                break;
            }
        }
        if (!collided && SHIFTDOWN(player.col) - player.horizontalSpeed > 0)
        {
            player.col -= SHIFTUP(player.horizontalSpeed);
        }
        player.direction = FACING_LEFT;
        player.animationState = ANIM_WALKING;
    }
    if (isButtonDown(BUTTON_RIGHT))
    {
        int collided = 0;
        for (i = 0; i < player.height; i++)
        {
            if (currentLevel->dimension[player.dimension]->dimensionCollisionMap->collisionMapData[OFFSET(SHIFTDOWN(player.row) + i, SHIFTDOWN(player.col) + player.horizontalSpeed + player.width, 256 * maxBackgrounds)] == 0)
            {
                collided = 1;
                break;
            }
        }
        if (!collided)
        {
            player.col += SHIFTUP(player.horizontalSpeed);
        }
        player.direction = FACING_RIGHT;
        player.animationState = ANIM_WALKING;
    }
    if (SHIFTDOWN(player.row) < 0)
    {
        player.row = 0;
    }
}

void setupPause()
{
    setupMode4();

    muteSound();

    loadPalette(pauseScreenPal);
}

void pause()
{
    drawBackgroundImage4(pauseScreenBitmap);

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_GAME;

        setupMode0();

        setupGameBackgrounds();
        setupSmokeBackground();
        setupGameBackgroundPalette();
        setupSprites();
        unmuteSound();
    }

    if (isButtonHit(BUTTON_SELECT))
    {
        currentState = STATE_GAME;

        setupMode0();

        setupGame();
    }
}

void setupWin()
{
    setupMode4();

    stopSound();

    loadPalette(winScreenPal);
}

void win()
{
    drawBackgroundImage4(winScreenBitmap);

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_MENU;

        fillScreen4(0);

        playSoundA(menuSong, MENUSONGLEN, MENUSONGFREQ, TRUE);

        setupMenu();
    }
}

void setupLose()
{
    setupMode4();

    stopSound();

    loadPalette(loseScreenPal);
}

void lose()
{
    drawBackgroundImage4(loseScreenBitmap);

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_MENU;

        fillScreen4(0);

        playSoundA(menuSong, MENUSONGLEN, MENUSONGFREQ, TRUE);

        setupMenu();
    }
}

void setupEntangled()
{
    setupMode4();

    stopSound();

    loadPalette(entangledScreenPal);
}

void entangled()
{
    drawBackgroundImage4(entangledScreenBitmap);

    flipPage();

    waitForVBlank();

    if (isButtonHit(BUTTON_START))
    {
        currentState = STATE_MENU;

        fillScreen4(0);

        playSoundA(menuSong, MENUSONGLEN, MENUSONGFREQ, TRUE);

        setupMenu();
    }
}

void setupMode0()
{
    REG_DISPCTL = MODE_0 | BG0_ENABLE |BG1_ENABLE | OBJ_ENABLE;

    REG_BG0CNT = BG_SIZE1 | SBB(30) | CBB(0) | 3;

    REG_BG1CNT = BG_SIZE0 | SBB(28) | CBB(2);
}

void setupMode4()
{
    REG_DISPCTL = MODE_4 | BG2_ENABLE;
}
