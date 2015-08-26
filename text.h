#ifndef TEXT_H
#define TEXT_H

#include <string.h>

extern const unsigned char fontdata_6x8[12288];

void drawChar(int x, int y, int ch, unsigned short color);
void drawString(int x, int y, char str[], unsigned short color);
void drawStringCentered(int y, char str[], unsigned short color);

#endif