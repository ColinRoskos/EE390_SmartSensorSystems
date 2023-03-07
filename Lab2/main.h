/* Main Header
 * Lab 2 |  | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
*/

#include "stdint.h"
#include "stdlib.h"
#include "math.h"

#include "stm32l475xx.h"

#include "../STM32Lib/sysclk.h"
#include "../STM32Lib/usart.h"
#include "../STM32Lib/system.h"
#include "../STM32Lib/dma.h"
#include "../STM32Lib/dac.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define SINE_SIZE 240

void memSine(uint16_t* data, uint16_t size);
