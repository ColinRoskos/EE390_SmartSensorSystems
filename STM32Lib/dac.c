
#include "dac.h"

void initDAC1DMA()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	NOP; // delay for unlocking GPIOA
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	NOP; // delay for unlocking DAC1

	// Setting DAC Control Register
	// TSEL = 0 for Timer 6, 
	DAC->CR = DAC_CR_TEN1 + DAC_CR_DMAEN1;// TEN1 Timer enable Channel 1, Enable DMA for channel 1
	
	GPIOA->MODER |= GPIO_MODER_MODE4; // set mode to analog
	DAC->MCR = 0; // set both DAC channel to normal mode with buffer enabled (also sets DAC2)
	DAC1->CR |= 1; // enable DAC channel 1
}

void initDAC1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	NOP; // delay for unlocking GPIOA
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	NOP; // delay for unlocking DAC1
	
	GPIOA->MODER |= GPIO_MODER_MODE4; // set mode to analog
	
/*DAC Channel 1 in Normal mode
	000: DAC Channel 1 is connected to external pin with Buffer enabled
	001: DAC Channel 1 is connected to external pin and to on chip peripherals with Buffer
					enabled
	010: DAC Channel 1 is connected to external pin with Buffer disabled
	011: DAC Channel 1 is connected to on chip peripherals with Buffer disabled
*/
	
	DAC->MCR = DAC_MCR_MODE1_0; // set both DAC channel to normal mode with buffer enabled
	DAC1->CR |= 1; // enable DAC channel 1
}


void dacOutput(uint16_t outputValue)
{
	DAC->DHR12R1 = outputValue;
}
