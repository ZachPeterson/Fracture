#ifndef TIMERLIB_H
#define TIMERLIB_H

// Timer control addresses
#define REG_TM0CNT *(volatile unsigned short*)0x4000102
#define REG_TM1CNT *(volatile unsigned short*)0x4000106
#define REG_TM2CNT *(volatile unsigned short*)0x400010A
#define REG_TM3CNT *(volatile unsigned short*)0x400010E

// Timer data addresses
#define REG_TM0D *(volatile unsigned short*)0x4000100
#define REG_TM1D *(volatile unsigned short*)0x4000104
#define REG_TM2D *(volatile unsigned short*)0x4000108
#define REG_TM3D *(volatile unsigned short*)0x400010C

// Timer flags
#define TIMER_ON      (1 << 7)  
#define TM_IRQ        (1 << 6)
#define TM_CASCADE    (1 << 2)
#define TM_FREQ_1     0
#define TM_FREQ_64    1
#define TM_FREQ_256   2
#define TM_FREQ_1024  3

#endif
