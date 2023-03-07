/* 
 * DMA Header for use with STM32
 * Author Colin Roskos 
*/
#ifndef DMA_H
#define DMA_H

#include "stdint.h"
#include "stm32l475xx.h"
#include "system.h"

#define DMA_C1SEL 1
#define DMA_C2SEL 2
#define DMA_C3SEL 3
#define DMA_C4SEL 4
#define DMA_C5SEL 5
#define DMA_C6SEL 6
#define DMA_C7SEL 7

void initDMAM2P(void * DMAFromMemBuffer, void * DMAToMemBuffer, uint16_t buffSize);
void setDMA1ClkSource(void);
void setDMAChannelSelect(uint8_t channel, uint8_t map);
uint16_t DMAcheck(void);

#endif
