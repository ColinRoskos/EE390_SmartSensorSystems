/*
* Lab 4 Header file
*
* Author Colin Roskos 
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
#include "../STM32Lib/dma.h"
#include "../STM32Lib/system.h"
#include "../STM32Lib/i2c.h"
#include "../STM32Lib/STDisco/HTS221.h"

#define MESSAGE_SPI_CONVERSION "Humidity: %2.1f%%  Temperature = %2.1f "
#define MESSAGE_DEG "C\n\r"

void sendTHtoUSART(double temp, double hum);

#endif
