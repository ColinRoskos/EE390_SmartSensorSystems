/*
* Lab 3 Header file
*
*
*/
#ifndef __main__
#define __main__

#include "stdint.h"
#include "stdlib.h"
#include "math.h"

#include "stm32l475xx.h"

#include "../STM32Lib/sysclk.h"
#include "../STM32Lib/usart.h"
#include "../STM32Lib/system.h"
#include "../STM32Lib/dma.h"
#include "../STM32Lib/dac.h"
#include "../STM32Lib/adc.h"

#define USER_INPUT_REQUEST "\n\rPlease enter DAC value (0-4095): "
#define USER_INPUT_DENY "\n\rEntered value is in incorrect format, please enter a value with only numbers\nin format 0-4095"
#define MESSAGE_DAC_CONVERSION "\rDAC1 convert %s to voltage."
#define MESSAGE_ADC_CONVERSION "\n\rADC conversion result is %s."
#define CLEAR_LINE "\r                                                                 "
#define UP_LINE "\033[1A"


void dacOutput(uint16_t);
uint32_t inputValidation(char *);

#endif
