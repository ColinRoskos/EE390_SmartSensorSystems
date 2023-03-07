/*
*  Lab 3 | DAC output | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
 * Write a program to configure DAC1 and ADC2 to function properly. 
 * For DAC1, feed the data to be converted using 12-bit right-aligned 
 * format. The DAC1 output is the channel 17 input of ADC2. Set VREF 
 * to 3.3 V (connect to VDD). 
 *
*/

#include "main.h"

static uint16_t dacValue;
static uint16_t adcValue;

int main(void)
{
	
	sysClkInit();
	initUSART1();
	initDAC1();
	initADC2();
	
	
	dacValue = 200;
	for(;;)
	{
		/*
		if (!pollUSART1(pRecBuff)){}
		else if (!inputValidation(pRecBuff))
		{
			putsUSART1(USER_INPUT_DENY);
			putsUSART1(USER_INPUT_REQUEST);
		}
		else
		{
		*/
			char tempDACBuff[32] = {'\0'};
			char tempADCBuff[32] = {'\0'};
			dacOutput(dacValue);
			
			putsUSART1(CLEAR_LINE);
			putsUSART1(UP_LINE);
			putsUSART1(CLEAR_LINE);
			uinttstr(tempDACBuff, dacValue);
			insertsUSART1(MESSAGE_DAC_CONVERSION, tempDACBuff);
			
			wait_ms_80MHz(500); // wait half second
			
		
			adcValue = getADC2();
		
			uinttstr(tempADCBuff, adcValue);
			insertsUSART1(MESSAGE_ADC_CONVERSION, tempADCBuff);
			wait_ms_80MHz(500); // wait half second
		
			dacValue = (dacValue < 4000) ? dacValue + 200 : 0;
		//}
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
