
#include "adc.h"

void initADC2(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	NOP; // delay for unlocking ADC
	
	// Channel 17 input ADC2, this is the internal channel connected to DAC1
	
	
	ADC2->CR	&= ~ADC_CR_DEEPPWD; 	// get out of deep power down mode
	ADC2->CR	|= 	ADC_CR_ADVREGEN; 	// enable ADC voltage regulator
	
	wait_us_80MHz(40); 	// wait for regulator to be stabilized ; T_ADCVREG_STUP = 20us

	// Set ADC2_CCR CH17SEL
	ADC123_COMMON->CCR |= ADC_CCR_TSEN_Msk; // TSEN, Temp Sensor
	ADC123_COMMON->CCR &= ~(ADC_CCR_CKMODE_Msk + ADC_CCR_MDMA_Msk);
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
	ADC2->ISR |= ADC_ISR_ADRDY; // write 1 to ISR ADRDY, clears ADC2_ISR
	ADC2->CR |= ADC_CR_ADEN;	// enable ADC2
	while(!(ADC2->ISR & ADC_ISR_ADRDY)); 	// wait for ADC2 to become ready
	
	// channel 17 diffsel is forced single ended mode
	
	ADC2->SQR1 = ADC_SQR1_L_0 +
							 ADC_SQR1_SQ1_4 + ADC_SQR1_SQ1_0;
	
	/*
		000: 2.5 ADC clock cycles
		001: 6.5 ADC clock cycles
		010: 12.5 ADC clock cycles
		011: 24.5 ADC clock cycles
		100: 47.5 ADC clock cycles
		101: 92.5 ADC clock cycles
		110: 247.5 ADC clock cycles
		111: 640.5 ADC clock cycles
	*/
	ADC2->SMPR2	|= 	0x1 << (7*3); 	// set channel 17 sample time to 640.5 ADC clock cycles

	ADC2->CFGR	= 0; 	// software trigger, right aligned, 12-bit resolution
	
	return;
}


void initADC1(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	NOP; // delay for unlocking ADC
	
	// Channel 17 input ADC2, this is the internal channel connected to DAC1
	
	
	ADC1->CR	&= ~ADC_CR_DEEPPWD; 	// get out of deep power down mode
	ADC1->CR	|= 	ADC_CR_ADVREGEN; 	// enable ADC voltage regulator
	
	wait_us_80MHz(40); 	// wait for regulator to be stabilized ; T_ADCVREG_STUP = 20us

	// Set ADC2_CCR CH17SEL
	ADC123_COMMON->CCR |= ADC_CCR_TSEN_Msk; // TSEN, Temp Sensor
	ADC123_COMMON->CCR &= ~(ADC_CCR_CKMODE_Msk + ADC_CCR_MDMA_Msk);
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
	ADC1->ISR |= ADC_ISR_ADRDY; // write 1 to ISR ADRDY, clears ADC2_ISR
	ADC1->CR |= ADC_CR_ADEN;	// enable ADC2
	while(!(ADC1->ISR & ADC_ISR_ADRDY)); 	// wait for ADC2 to become ready
	
	// channel 17 diffsel is forced single ended mode
	
	ADC1->SQR1 = ADC_SQR1_L_0 +
							 ADC_SQR1_SQ1_4 + ADC_SQR1_SQ1_0;
	
	/*
		000: 2.5 ADC clock cycles
		001: 6.5 ADC clock cycles
		010: 12.5 ADC clock cycles
		011: 24.5 ADC clock cycles
		100: 47.5 ADC clock cycles
		101: 92.5 ADC clock cycles
		110: 247.5 ADC clock cycles
		111: 640.5 ADC clock cycles
	*/
	ADC1->SMPR2	|= 	0x1 << (7*3); 	// set channel 17 sample time to 640.5 ADC clock cycles + 12.5 ADC clock cycles (12 bit resolution) == 653 ADC Clock Cycles

	ADC1->CFGR	= 0; 	// software trigger, right aligned, 12-bit resolution
	
	return;
}

uint16_t getADC1(void)
{
	static volatile uint16_t dr;
	
	ADC1->CR |= ADC_CR_ADSTART; // start a new A/D conversion
	wait_us_80MHz(100); // 1us ~= 80 clock cycles - 800 clock cycles
	while(!(ADC1->ISR & ADC_ISR_EOC)); // wait for ADC to complete
	
	dr = (uint16_t) ADC1->DR;//*((uint16_t*) 0x50040040);// 

	wait_us_80MHz(10000); // 1us ~= 80 clock cycles - 800 clock cycles
	return dr;
}	

uint16_t getADC2(void)
{
	ADC2->CR |= ADC_CR_ADSTART; // start a new A/D conversion
	wait_us_80MHz(9); // 1us ~= 80 clock cycles
	while(!(ADC2->ISR & ADC_ISR_EOC)); // wait for ADC to complete
	
	return (uint16_t) ADC2->DR;
}	
