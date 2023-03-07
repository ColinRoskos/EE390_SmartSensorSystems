/* 
 * ADC Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef ADC_H
#define ADC_H

#include "stm32l475xx.h"
#include "system.h"
#include "sysclk.h"

void initADC2(void);
void initADC1(void);
uint16_t getADC2(void);
uint16_t getADC1(void);

#endif
