/* 
 * I2C Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef I2C_H
#define I2C_H

#include "stdbool.h"
#include "stm32l475xx.h"
#include "sysclk.h"

void I2C2_EV_IRQHandler(void);
void writePoll (uint8_t);
void initI2C(void);

#endif
