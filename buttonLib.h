#ifndef BUTTONLIB_H
#define BUTTONLIB_H

// #define for button register
#define BUTTON_ADDRESS (*((unsigned short *) 0x4000130))

// #defines for detecting key presses
#define KEY_MASK 0x03FF
#define BUTTON_HELD(key) (~BUTTON_ADDRESS & (key))

// #defines for button codes
#define BUTTON_A         (1 << 0)
#define BUTTON_B         (1 << 1)
#define BUTTON_SELECT    (1 << 2)
#define BUTTON_START     (1 << 3)
#define BUTTON_RIGHT     (1 << 4)
#define BUTTON_LEFT      (1 << 5)
#define BUTTON_UP        (1 << 6)
#define BUTTON_DOWN      (1 << 7)
#define BUTTON_R         (1 << 8)
#define BUTTON_L         (1 << 9)

// Functions to check button states
void pollButtons();
unsigned long getCurrentButtonState();
unsigned long getPreviousButtonState();
unsigned long isButtonDown(unsigned long Button);
unsigned long isButtonUp(unsigned long Button);
unsigned long wasButtonDown(unsigned long Button);
unsigned long wasButtonUp(unsigned long Button);
unsigned long isButtonHeld(unsigned long Button);
unsigned long isButtonHit(unsigned long Button);
unsigned long isButtonReleased(unsigned long Button);

#endif
