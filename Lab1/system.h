#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdint.h"


#define SUCCESS 1
#define FAILURE 0

#define UART1 	1

#define	ENTER	0x0D	// ASCII code of the ENTER character
#define	BS	0x08	// ASCII code of the BACKSPACE character
#define	ASCII_BS	BS	// ASCII code of the BACKSPACE character
#define	SPACE	0x20	// ASCII code of the SPACE character
#define	ASCII_SP SPACE	// ASCII code of the BACKSPACE character
#define ASCII_CR 0x0D// Carriage Return
#define ASCII_LF 0x0A// Line Feed
#define ASCII_EOL 0x00 // End of Line
#define ASCII_COLON 0x3A
#define ASCII_ZERO 0x30
#define ASCII_A 0x41
#define ASCII_HEX_OFFSET 0x37

char uint2char(uint32_t num);
void hex2char(char * hexBuf, uint16_t val);

#endif
