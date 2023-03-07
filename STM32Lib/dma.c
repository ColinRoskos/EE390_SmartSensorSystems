
#include "dma.h"

/*
 * initDMAM2P
 * inputs: 
 * 		DMAFromMemBuffer: pointer to from memory location
 * 		DMAToMemBuffer: pointer to destination memory location
 *		buffSize: buffer size of source local memory
 *
*/
void	initDMAM2P(void * DMAFromMemBuffer, void * DMAToMemBuffer, uint16_t buffSize)
{
	setDMA1ClkSource();
	
	DMA1_CSELR->CSELR &= ~(DMA_CSELR_C3S_Msk);
	DMA1_CSELR->CSELR |= 0x6 << DMA_CSELR_C3S_Pos; // b`0110 TIM6_UP/DAC1
	
	DMA1_Channel3->CMAR = (uint32_t) DMAFromMemBuffer;
	DMA1_Channel3->CPAR = (uint32_t) DMAToMemBuffer;
	DMA1_Channel3->CNDTR = buffSize;
	DMA1_Channel3->CCR = DMA_CCR_PL_1 + DMA_CCR_PL_0 +  						// DMA High Priority
												DMA_CCR_MSIZE_0 +   											// 16-bit local memory size
												DMA_CCR_PSIZE_0 + DMA_CCR_MINC +  				// 16-bit destination memory size, increment
												DMA_CCR_CIRC + DMA_CCR_DIR + DMA_CCR_EN;  // circular, out from device, enable
}


void setDMA1ClkSource(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	NOP; // delay one clock cycle to keep RCC change
}

uint16_t DMAcheck()
{
	//TODO
	return 1;
}
