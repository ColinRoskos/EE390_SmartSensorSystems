/* 
 * DAC Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef DAC_H
#define DAC_H

#include "stm32l475xx.h"
#include "system.h"

#define DAC_TRIGGER_TIMER2 4
#define DAC_TRIGGER_TIMER4 5
#define DAC_TRIGGER_TIMER5 3
#define DAC_TRIGGER_TIMER6 0
#define DAC_TRIGGER_TIMER7 2
#define DAC_TRIGGER_TIMER8 1
#define DAC_TRIGGER_EXTERN 6 // External line 9
#define DAC_TRIGGER_SOFTWARE 7
#define DAC_TRIGGER_DISABLE 8

#define DAC_CHANNEL1_SEL 0
#define DAC_CHANNEL2_SEL 1

/* Prototype functions */

void initDAC1DMA(void);
void initDAC1(void);
void setDACCRTriggered(uint8_t channel, uint8_t trigger);
void sineDAC(uint16_t * sineWave, uint16_t lenSine);
void testDAC(void);
void dacOutput(uint16_t);

#endif
