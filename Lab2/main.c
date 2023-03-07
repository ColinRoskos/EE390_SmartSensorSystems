/* 
 * Lab 2 | DAC with DMA Transfer | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
 * Write a program to generate a sine wave (output from PA4 pin). Divide 
 * a cycle of the sine wave into 60 points. Use DMA transfer to send the 
 * data to be converted to the DAC1. Use external trigger (TIM6) to 
 * trigger the DMA transfer every 4 ?s. The DMA must be configured to 
 * circular mode.
 * 
*/


#include "main.h"

void initClk(void);

static uint16_t sineWave[SINE_SIZE];
static uint16_t  returnVal = SUCCESS;

int main()
{
	returnVal = 1;
	
	memSine(sineWave, SINE_SIZE); // setup sine wave buffer with values
	
	sysClkInit(); // init system clock to 80MHz, HSI
	
	initDAC();
	initTimer6TriggerDMA(1); // initialize trigger with a wait of 4 us
	initDMAM2P(sineWave, &(DAC1->DHR12R1), SINE_SIZE); // init DMA from mem SineWave to DAC1 12 bit right justified
	
	while(returnVal == SUCCESS)
	{
		returnVal = DMAcheck(); // A while loop with a TODO DMA check
	}
	
	return returnVal;
}

/*
 * memSine
 * inputs: data: pointer to 16 bit data array, size: size of array
 * 
 * Creates a 12-bit sine wave with /size/ number of datapoints
*/
void memSine(uint16_t* data, uint16_t size)
{
	double inter;
	for(int i = 0 ; i < size ; i++)
	{
		inter = (((double) i)/((float)size)) * 2.0 * M_PI;
		inter = (1.0 + (sin(inter)*.98))/2.0;
		data[i] = (uint16_t)(inter * 0xfff);
	}
}
