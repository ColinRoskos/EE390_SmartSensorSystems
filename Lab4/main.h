/*
* Lab 4 Header file
*
*
*/
#ifndef __main__
#define __main__


#include <stdio.h>
#include <stdbool.h>
#include "stdint.h"
#include "stdlib.h"
#include "math.h"

#include "stm32l475xx.h"

#include "../STM32Lib/sysclk.h"
#include "../STM32Lib/usart.h"
#include "../STM32Lib/system.h"
#include "../STM32Lib/adc.h"

#define USER_INPUT_REQUEST "\n\rPlease enter DAC value (0-4095): "
#define USER_INPUT_DENY "\n\rEntered value is in incorrect format, please enter a value with only numbers\nin format 0-4095"

#define MESSAGE_ADC_CONVERSION "Temperature = %.1f\n\r"
#define TEMP_MESSAGE_ADC_CONVERSION "Temperature = %d\n\r"


void dacOutput(uint16_t);
uint32_t inputValidation(char *);

#endif
