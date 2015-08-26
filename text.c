#include "text.h"

void drawChar(int x, int y, int ch, unsigned short color)
{
    int r, c;
    for (r = 0; r < 8; r++)
    {
        for (c = 0; c < 6; c++)
        {
            if (fontdata_6x8[((r * 6) + c) + (ch * 48)])
            {
                setPixel(x + c, y + r, color);
            }
        }
    }
}

void drawString(int x, int y, char str[], unsigned short color)
{
    while (*str)
    {
        drawChar(x, y, *str++, color); // Dereference the pointer, finish the statement, then increment the pointer
        x += 6;
    }
}

void drawStringCentered(int y, char str[], unsigned short color)
{
    int x = (240 - (6 * strlen(str))) / 2;
    while (*str)
    {
        drawChar(x, y, *str++, color); // Dereference the pointer, finish the statement, then increment the pointer
        x += 6;
    }
}

void drawStringLeftAligned(int leftPadding, int y, char str[], unsigned short color)
{
    int x = leftPadding;
    while (*str)
    {
        drawChar(x, y, *str++, color); // Dereference the pointer, finish the statement, then increment the pointer
        x += 6;
    }
}

void drawStringRightAligned(int rightPadding, int y, char str[], unsigned short color)
{
    int x = (240 - (6 * strlen(str))) - rightPadding;
    while (*str)
    {
        drawChar(x, y, *str++, color); // Dereference the pointer, finish the statement, then increment the pointer
        x += 6;
    }
}
