#ifndef MYLIB_H
#define	MYLIB_H

// Include standard library files
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

// ----------------------------------------
// Function declarations
// ----------------------------------------

// Used to delay for a specified amount
void delay(int n);

// Checks whether two bounding boxes intersect
// Uses top left corner, width and height for each one
// Returns TRUE if the two boxes are colliding
bool checkAABBCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

#endif

