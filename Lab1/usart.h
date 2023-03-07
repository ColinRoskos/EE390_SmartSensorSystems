/*
* header for usart
*/

#ifndef USART_H
#define USART_H

#define UART1 1

#include "stm32l475xx.h"
#include "stdint.h"
#include "system.h"


void initUART (void);
void putsUART(char* msgRec);
void putcUART(char msgChar);
void getsUART(char* msg);
char getcUART(void);
void putNewLineUART(void);
void _bsUART(void);
void backspaceUART(uint16_t num);

#endif
