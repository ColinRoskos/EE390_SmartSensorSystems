/* 
 * Generic Helper Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdlib.h"
#include "stdint.h"
#include "math.h"

#define SUCCESS 1
#define FAILURE 0

#define NOP __asm("NOP")

#define UART1 	1

#define	ENTER	0x0D	// ASCII code of the ENTER character
#define	BS	0x08	// ASCII code of the BACKSPACE character
#define	ASCII_BS	BS	// ASCII code of the BACKSPACE character
#define ASCII_ESC 0x1B // ASCII escape character
#define	SPACE	0x20	// ASCII code of the SPACE character
#define	ASCII_SP SPACE	// ASCII code of the BACKSPACE character
#define ASCII_CR 0x0D// Carriage Return
#define ASCII_LF 0x0A// Line Feed
#define ASCII_EOL 0x00 // End of Line
#define ASCII_COLON 0x3A // ':' character
#define ASCII_HEX_OFFSET 0x37
#define ASCII_DEL 0x7F // ASCII code of the DELETE character
#define ASCII_NUM 0x30 // ASCII numbers start, index of ASCII_ZERO
#define ASCII_ZERO ASCII_NUM // index of ascii zero '0'
#define ASCII_CHAR 0x41 // ASCII letters start, index of ascii 'A'
#define ASCII_A ASCII_CHAR // index of ascii 'A'
#define FORMAT_DELIMITTER 0x25 // index of ascii '%'
#define ASCII_PERCENT 0xF8 // index of ascii '°'

#define TEMP_CONV_HIGH 110.0
#define TEMP_CONV_LOW 30.0



char uint2char(uint32_t);
char hex2char(uint8_t);
void uinttstr(char *, uint32_t);
uint32_t strtuint(char *);
double tempConversion(uint16_t);

#endif
