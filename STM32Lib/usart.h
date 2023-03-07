/* 
 * USART Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef USART_H
#define USART_H

#include "stm32l475xx.h"
#include "system.h"

#define UART1 1

#define CLEAR_LINE "\r                                                                 \r"
#define UP_LINE "\033[1A"

void initUSART1 (void);
uint32_t pollUSART1(char *);
void putsUSART1(char *);
void puthUSART1(uint16_t);
void putuUSART1(uint32_t);
void putcUSART1(char);
void getsUSART1(char *);
char getcUSART1(void);
void putNewLineUSART1(void);
void _bsUSART1(void);
void backspaceUSART1(uint16_t);
void printMemory(uint32_t *, uint32_t);
void _printMemoryStrPortion(char *);
void insertsUSART1(char *, char *);

#endif
