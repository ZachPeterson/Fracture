#include "dmaLib.h"

DMA *dma = (DMA *)0x40000B0;

// Function to call DMA on the specified channel
void DMANow(int channel, void* source, void* destination, unsigned int control)
{
	dma[channel].src = source;
	dma[channel].dst = destination;
	dma[channel].cnt = DMA_ON | control;
}
