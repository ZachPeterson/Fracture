#include "buttonLib.h"

// Various functions and declarations to check buttons
extern unsigned short currentButtonState, previousButtonState;
unsigned short currentButtonState = 0, previousButtonState = 0;

inline void pollButtons() {
	previousButtonState = currentButtonState;
	currentButtonState = (~BUTTON_ADDRESS & KEY_MASK);
}

unsigned long getCurrentButtonState()
{
    return currentButtonState;
}

unsigned long getPreviousButtonState()
{
    return previousButtonState;
}

unsigned long isButtonDown(unsigned long Button)
{
    return currentButtonState & Button;
}

unsigned long isButtonUp(unsigned long Button)
{
    return ~currentButtonState & Button;
}

unsigned long wasButtonDown(unsigned long Button)
{
    return previousButtonState & Button;
}

unsigned long wasButtonUp(unsigned long Button)
{
    return ~previousButtonState & Button;
}

unsigned long isButtonHeld(unsigned long Button)
{
    return (currentButtonState & previousButtonState) & Button;
}

unsigned long isButtonHit(unsigned long Button)
{
    return (currentButtonState & ~previousButtonState) & Button;
}

unsigned long isKeyReleased(unsigned long Button)
{
    return (~currentButtonState & previousButtonState) & Button;
}
