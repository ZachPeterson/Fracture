#include "myLib.h"

// Used to delay for a specified amount
void delay(int n)
{
    int i;
    volatile int b;
    for(i = 0; i < 10000 * n; i++) {
        b++;
    }
}

// Checks whether two bounding boxes intersect
// Uses top left corner, width and height for each one
// Returns TRUE if the two boxes are colliding
bool checkAABBCollision(volatile int x1,
                        volatile int y1,
                        volatile int width1,
                        volatile int height1,
                        volatile int x2,
                        volatile int y2,
                        volatile int width2,
                        volatile int height2)
{
    if ((x1 + width1) < (x2))
    {
        return FALSE;
    }
    if ((x1) > (x2 + width2))
    {
        return FALSE;
    }
    if ((y1 + height1) < (y2))
    {
        return FALSE;
    }
    if ((y1) > (y2 + height2))
    {
        return FALSE;
    }
    return TRUE;
}
