

#include "system.h"

/*
*  int2char
*  param uint32_t
*  returns char
*
*  takes an unsigned integer returns the least significant digit as a character
*/
char uint2char(uint32_t num)
{
	return (char) (num % 10) + ASCII_ZERO;
}

char hex2char(uint8_t val)
{
	uint8_t inter = val & 0xf;
	return (inter < 10) ? uint2char(val) : (inter + ASCII_HEX_OFFSET);
}

void uinttstr(char * pStr, uint32_t num)
{
	uint32_t inter = num;
	uint8_t digit = 0;
	uint8_t index = 0;
	char interStr[10];
	while(inter) // not zero
	{
		interStr[digit++] = uint2char(inter%10);
		inter = inter / 10;
	}
	if (digit == 0)
	{
		pStr[digit++] = ASCII_ZERO;
		return;
	}
	
	while(digit)
	{
		pStr[index++] = interStr[--digit];
	}
	return;
}


static uint16_t TS_CAL1;
static uint16_t TS_CAL2;

double tempConversion(uint16_t TS_DATA)
{
	static volatile double num; // static volotile to prevent compiler shenanigans 
	static volatile double den;
	static volatile double var;
	static volatile double itr;
	
	if(!TS_CAL1)
		TS_CAL1 = *((uint16_t*) 0x1FFF75A8); //https://www.st.com/resource/en/datasheet/DM00172872.pdf page 39
	if(!TS_CAL2)
		TS_CAL2 = *((uint16_t*) 0x1FFF75CA);
	
	num = (double)(TEMP_CONV_HIGH-TEMP_CONV_LOW); // compiler will discard these constants without 'static volatile'
	den = (double)(TS_CAL2-TS_CAL1);
	var = (double)(TS_DATA-TS_CAL1);
	itr = (double)TEMP_CONV_LOW;
		
	return (num/den)*var + itr;
}
