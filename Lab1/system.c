

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

void hex2char(char * hexBuf, uint16_t val)
{
	uint16_t inter = val;
	for (uint8_t i ; i < 4 ; i++)
	{
		hexBuf[i] = ((inter & 0xf) < 10) ? uint2char(inter) : (ASCII_HEX_OFFSET + (inter & 0xf));
	}
}
