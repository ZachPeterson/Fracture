#include "dmaLib.h"
#include "myLib.h"
#include "videoLib.h"

unsigned short *videoBuffer = (unsigned short*)0x6000000;

unsigned short *frontBuffer = (unsigned short*)0x6000000;
unsigned short *backBuffer =  (unsigned short*)0x600A000;

// Function to wait until VBlank
// Used to time drawing so that it only occurs during VBlank
void waitForVBlank()
{
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}

// Used to set a specified pixel to a specified color
void setPixel(int x, int y, unsigned short color)
{
    VIDEO_BUFFER[OFFSET((y), (x), (240))] = color;
}

void setPixel4(int row, int col, unsigned char colorIndex)
{
	unsigned short pixels = videoBuffer[OFFSET(row, col/2, 240/2)];

	if(col % 2 == 0) // even
	{
		pixels &= 0xFF << 8;
		videoBuffer[OFFSET(row, col/2, 240/2)] = pixels | colorIndex;
	}
	else // odd
	{
		pixels &= 0xFF;
		videoBuffer[OFFSET(row, col/2, 240/2)] = pixels | colorIndex << 8;
	}
}

void fillScreen4(unsigned char colorIndex)
{
	volatile unsigned short pixels = colorIndex << 8 | colorIndex;
	DMANow(3, &pixels, videoBuffer, ((240 * 160)/2) | DMA_SOURCE_FIXED);
}

void drawBackgroundImage4(const unsigned short* image)
{
    DMANow(3, (unsigned short*)image, videoBuffer, ((240 * 160)/2));
}

void drawImage4(const unsigned short* image, int row, int col, int height, int width)
{
    if(col%2)
    {
        col++;
    }

    int r;
    for(r = 0; r < height; r++)
    {
        DMANow(3, (unsigned short*)&image[OFFSET(r,0,width/2)], &videoBuffer[OFFSET(row + r, col/2, 240/2)], width/2);
    }
}

void flipPage()
{
    if(REG_DISPCTL & BUFFER2)
    {
        REG_DISPCTL &= ~BUFFER2;
        videoBuffer = backBuffer;
    }
    else
    {
        REG_DISPCTL |= BUFFER2;
        videoBuffer = frontBuffer;
    }
}


// Draws a rectangle with a specified position, dimensions and color
// If wrapRectangle is true, then the rectangle will be wrapped around the screen if part of it would be draw off screen
// Otherwise, any pixels outside the right side of the screen are wrapped around the left side
void drawRectangle(int x, int y, int width, int height, volatile unsigned short color, bool wrapRectangle)
{
    int i;
    // Make sure the width is 240 or less, as it should not wrap multiple times
    int moddedWidth = width % 241;
    for (i = 0; i < height; i++)
    {
        if (wrapRectangle)
        {
            if (x + moddedWidth > COLUMNS)
            {
                int dmaWidth = moddedWidth;
                int dmaWidth2 = 0;
                dmaWidth = 240 - x;
                dmaWidth2 = moddedWidth - dmaWidth;
                /*
                DMA[3].src = &color;
                DMA[3].dst = &VIDEO_BUFFER[OFFSET(y + i, x)];
                DMA[3].cnt = dmaWidth | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_NOW | DMA_ON | DMA_16;
                */
                DMANow(3, &color, &VIDEO_BUFFER[OFFSET(y + i, x, 240)], dmaWidth | DMA_SOURCE_FIXED | DMA_ON);
                /*
                DMA[3].src = &color;
                DMA[3].dst = &VIDEO_BUFFER[OFFSET(y + i, 0)];
                DMA[3].cnt = dmaWidth2 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_NOW | DMA_ON | DMA_16;
                */
                DMANow(3, &color, &VIDEO_BUFFER[OFFSET(y + i, 0, 240)], dmaWidth2 | DMA_SOURCE_FIXED | DMA_ON);
            }
            else
            {
                /*
                DMA[3].src = &color;
                DMA[3].dst = &VIDEO_BUFFER[OFFSET(y + i, x)];
                DMA[3].cnt = moddedWidth | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_NOW | DMA_ON | DMA_16;
                */
                DMANow(3, &color, &VIDEO_BUFFER[OFFSET(y + i, x, 240)], moddedWidth | DMA_SOURCE_FIXED | DMA_ON);
                
            }
        }
        else
        {
            int dmaWidth = moddedWidth;
            if (x + moddedWidth > COLUMNS)
            {
                dmaWidth = 240 - x;
            }
            /*
            DMA[3].src = &color;
            DMA[3].dst = &VIDEO_BUFFER[OFFSET(y + i, x)];
            DMA[3].cnt = dmaWidth | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_NOW | DMA_ON | DMA_16;
            */
            DMANow(3, &color, &VIDEO_BUFFER[OFFSET(y + i, x, 240)], dmaWidth | DMA_SOURCE_FIXED | DMA_ON);
        }
    }
}

void loadTiles(const unsigned short* tiles, int characterBlockNumber, int tilesLength)
{
    DMANow(3, tiles, CHARACTERBLOCKBASE + characterBlockNumber, (tilesLength / 2));
}

void loadTileMap(const unsigned short* map, int screenBlockNumber, int mapLength)
{
    DMANow(3, map, SCREENBLOCKBASE + screenBlockNumber, (mapLength / 2));
}

void loadPalette(const unsigned short* palette)
{
    DMANow(3, palette, PALETTE, 256);
}
