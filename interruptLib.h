#ifndef INTERRUPTLIB_H
#define INTERRUPTLIB_H

// The master interrupt enable register
#define REG_IME *(unsigned short *)0x04000208

// The interrupt enable register to enable specific interrupts
#define REG_IE *(unsigned short *)0x04000200

// Interrupt flag register to determine which interrupt occurred
#define REG_IF *(volatile unsigned short *)0x04000202

// The register to store the address of the interrupt handler function
#define REG_INTERRUPT *(unsigned long *)0x03007FFC

// The register to enable display based interrupts
#define REG_DISPSTAT *(unsigned short *)0x04000004

// #defines for the different display interrupt enable bits
// These should be put into REG_DISPSTAT
#define INT_VBLANK_ENABLE (1 << 3)
#define INT_HBLANK_ENABLE (1 << 4)
#define INT_VCOUNT_ENABLE (1 << 5)

// #defines for checking which type of interrupt happened
#define INT_VB     (1 << 0)
#define INT_HB     (1 << 1)
#define INT_VC     (1 << 2)
#define INT_T0     (1 << 3)
#define INT_T1     (1 << 4)
#define INT_T2     (1 << 5)
#define INT_T3     (1 << 6)
#define INT_COM    (1 << 7)
#define INT_DMA0   (1 << 8)
#define INT_DMA1   (1 << 9)
#define INT_DMA2   (1 << 10)
#define INT_DMA3   (1 << 11)
#define INT_BUTTON (1 << 12)
#define INT_CART   (1 << 13)

void setupInterrupts();
void gameInterruptHandler();

#endif
