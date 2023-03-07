/*
*  Lab 4 | DAC output | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
 * Write a program to configure ADC1 to measure the on-chip 
 *  temperature using 12-bit resolution. For ADC1, VTS is 
 *  assigned to channel 17. The measurement is performed 
 *  once every second. 
 *
*/

#include "main.h"

static uint16_t dacValue;
static uint16_t adcValue;
static char* buff[200];

int main(void)
{
	
	sysClkInit();
	initUSART1();
	initADC1();
	
	bool tick = false;
	
	for(;;)
	{
			//float adcConversion;
			
			char tickMark = ASCII_SP;
			
			putsUSART1(CLEAR_LINE);
			putsUSART1(UP_LINE);
		
			adcValue = getADC1();
		
			double conv = tempConversion(adcValue);
			sprintf((char *)buff, MESSAGE_ADC_CONVERSION, conv);
			putsUSART1((char *)buff);
			
			if (tick)
				tickMark = '.';
			
			tick = !tick;
				
			
//			putcUSART1(tickMark);
			
			wait_ms_80MHz(1000); // wait one second

	}
	
	
	return 1;
	
}

uint32_t inputValidation(char * pMsgBuff)
{
	uint16_t ichar = 0; //character index
	char nChar = pMsgBuff[ichar];
	dacValue = 0;
	while(nChar == ASCII_EOL || ichar++ < 5) // cannot be above 4 values long, 0xFFF
	{
		if (nChar == ASCII_EOL) return 1; // if all values checked
		
		dacValue = (dacValue * 10) + uint2char(nChar);
		if (dacValue > 0xFFF) return 0;
		
		nChar = pMsgBuff[ichar];
		
	}
	
	return 0; // false
}
