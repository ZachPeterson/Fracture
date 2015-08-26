#include "dmaLib.h"
#include "soundLib.h"
#include "timerLib.h"
#include "videoLib.h"

void setupSound()
{
    REG_SOUNDCNT_X = SND_ENABLED;

    REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_TO_BOTH |
                     DSA_TIMER0 |
                     DSA_FIFO_RESET |
                     DSB_OUTPUT_RATIO_100 |
                     DSB_OUTPUT_TO_BOTH |
                     DSB_TIMER1 |
                     DSB_FIFO_RESET;

    REG_SOUNDCNT_L = 0;

    vbCountA = 0;
    vbCountB = 0;
}

void soundInterruptHandler()
{
    vbCountA++;
    vbCountB++;

    if (soundA.isPlaying && soundA.duration < vbCountA)
    {
        if (soundA.shouldLoop)
        {
            playSoundA(soundA.data, soundA.length, soundA.frequency, soundA.shouldLoop);
        }
        else
        {
            soundA.isPlaying = 0;
            REG_TM0CNT = 0;
            dma[1].cnt = 0;
        }
    }
    if (soundB.isPlaying && soundB.duration < vbCountB)
    {
        if (soundB.shouldLoop)
        {
            playSoundB(soundB.data, soundB.length, soundB.frequency, soundB.shouldLoop);
        }
        else
        {
            soundB.isPlaying = 0;
            REG_TM1CNT = 0;
            dma[2].cnt = 0;
        }
    }
}

void playSoundA(const unsigned char *sound, int length, int frequency, int shouldLoop)
{
    dma[1].cnt = 0;
    vbCountA = 0;

    int interval = 16777216 / frequency;

    DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    REG_TM0CNT = 0;

    REG_TM0D = -interval;
    REG_TM0CNT = TIMER_ON;

    soundA.data = sound;
    soundA.length = length;
    soundA.frequency = frequency;
    soundA.isPlaying = 1;
    soundA.shouldLoop = shouldLoop;
    soundA.duration = ((length * 60) / frequency) - ((length / frequency) * 3) - 1;
}

void playSoundB(const unsigned char *sound, int length, int frequency, int shouldLoop)
{
    dma[2].cnt = 0;
    vbCountB = 0;

    int interval = 16777216 / frequency;

    DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    REG_TM1CNT = 0;

    REG_TM1D = -interval;
    REG_TM1CNT = TIMER_ON;

    soundB.data = sound;
    soundB.length = length;
    soundB.frequency = frequency;
    soundB.isPlaying = 1;
    soundB.shouldLoop = shouldLoop;
    soundB.duration = ((length * 60) / frequency) - ((length / frequency) * 3) - 1;
}

void muteSound()
{
    REG_SOUNDCNT_X = 0;
}

void unmuteSound()
{
    REG_SOUNDCNT_X = SND_ENABLED;
}

void stopSound()
{
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;

    REG_TM0CNT = 0;
    dma[1].cnt = 0;

    REG_TM1CNT = 0;
    dma[2].cnt = 0;
}
