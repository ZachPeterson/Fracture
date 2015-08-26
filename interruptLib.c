#include "interruptLib.h"

void setupInterrupts()
{
	REG_IME = 0;
	REG_INTERRUPT = (unsigned int)gameInterruptHandler;
	REG_IE |= INT_VB;
	REG_DISPSTAT |= INT_VBLANK_ENABLE;
	REG_IME = 1;
}
