#ifndef VIDEOLIB_H
#define VIDEOLIB_H

// #defines for basic video control
#define REG_DISPCTL (*(unsigned short*) 0x4000000)
#define PALETTE ((unsigned short *)0x5000000)
#define VIDEO_BUFFER ((unsigned short*) 0x6000000)
#define MODE_0 0
#define MODE_3 3
#define MODE_4 4

extern unsigned short *videoBuffer;

extern unsigned short *buffer1;
extern unsigned short *buffer2;

// MODE 4 Drawing functions
void setPixel4(int row, int col,volatile unsigned char colorIndex);
void drawRect4(int row, int col, int height, int width,volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char color);
void drawBackgroundImage4(const unsigned short* image);
void drawImage4(const unsigned short* image, int row, int col, int height, int width);
void loadPalette(const unsigned short* palette);
void drawSubImage4(const unsigned short* sourceImage, int sourceRow, int sourceCol, int sourceWidth, int row, int col, int height, int width);

void flipPage();

#define BUFFER2 (1 << 4)

// Vblank frequency
#define VBLANKFREQUENCY 59.727

// #defines for screen size
#define ROWS 160
#define COLUMNS 240

// Offset #define for calculating which pixel to change in the video buffer
#define OFFSET(r,c,rowlen) ((r)*(rowlen)+(c))

// #define to pack color values into an unsigned short
#define COLOR(R, G, B) ((R) | (G)<<5 | (B)<<10)

// #define for the scanline counter so that drawing can be done during VBlank
#define SCANLINECOUNTER *(volatile unsigned short*)0x4000006

// #defines for background enable flags
#define BG0_ENABLE (1<<8)
#define BG1_ENABLE (1<<9)
#define BG2_ENABLE (1<<10)
#define BG3_ENABLE (1<<11)

// #defines for background control registers
#define REG_BG0CNT *(volatile unsigned short*)0x4000008
#define REG_BG1CNT *(volatile unsigned short*)0x400000A
#define REG_BG2CNT *(volatile unsigned short*)0x400000C
#define REG_BG3CNT *(volatile unsigned short*)0x400000E

// The structures needed to represent a character block and a screen block
typedef struct {
    unsigned short tileImage[8192];
} characterBlock;

typedef struct {
    unsigned short tileMap[1024];
} screenBlock;

#define CHARACTERBLOCKBASE ((characterBlock *)0x6000000)
#define SCREENBLOCKBASE ((screenBlock *)0x6000000)

// #defines for background offset registers
#define REG_BG0HOFS *(volatile unsigned short *)0x04000010
#define REG_BG1HOFS *(volatile unsigned short *)0x04000014
#define REG_BG2HOFS *(volatile unsigned short *)0x04000018
#define REG_BG3HOFS *(volatile unsigned short *)0x0400001C

#define REG_BG0VOFS *(volatile unsigned short *)0x04000012
#define REG_BG1VOFS *(volatile unsigned short *)0x04000016
#define REG_BG2VOFS *(volatile unsigned short *)0x0400001A
#define REG_BG3VOFS *(volatile unsigned short *)0x0400001E

// #defines for macros and bit constants for setting the background control register specifics
#define SBB(num) num << 8
#define CBB(num) num << 2
#define COLOR256 1 << 7
#define BG_SIZE0 0<<14 // 32 x 32
#define BG_SIZE1 1<<14 // 64 x 32
#define BG_SIZE2 2<<14 // 32 x 64
#define BG_SIZE3 3<<14 // 64 x 64

// #defines for alpha blending background stuff
#define REG_BLDCNT   *(volatile unsigned short *)0x04000050
#define REG_BLDALPHA *(volatile unsigned short *)0x04000052
#define REG_BLDY     *(volatile unsigned short *)0x04000054

// #defines for blending modes
#define BLD_OFF   (0 << 6)
#define BLD_STD   (1 << 6)
#define BLD_WHITE (2 << 6)
#define BLD_BLACK (3 << 6)

// #defines for background layers
#define ABG0 (1 << 0)
#define ABG1 (1 << 1)
#define ABG2 (1 << 2)
#define ABG3 (1 << 3)
#define AOBJ (1 << 4)
#define ABD  (1 << 5)
#define BBG0 (1 << 8)
#define BBG1 (1 << 9)
#define BBG2 (1 << 10)
#define BBG3 (1 << 11)
#define BOBJ (1 << 12)
#define BBD  (1 << 13)

// Function to wait until VBlank
// Used to time drawing so that it only occurs during VBlank
void waitForVBlank();

// Used to set a specified pixel to a specified color
void setPixel(int x, int y, unsigned short color);

// Draws a rectangle with a specified position, dimensions and color
// If wrapRectangle is true, then the rectangle will be wrapped around the screen if part of it would be draw off screen
// Otherwise, any pixels outside the right side of the screen are wrapped around the left side
void drawRectangle(int x, int y, int width, int height, unsigned short color, bool wrapRectangle);

// Loads a set of tiles into one of the 4 character block slots
void loadTiles(const unsigned short* tiles, int characterBlock, int tilesLength);

// Loads a tile map into one of the 32 screen block slots
void loadTileMap(const unsigned short* map, int screenBlock, int mapLength);

// Loads a palette
void loadPalette(const unsigned short* palette);

#endif
