
#ifndef SYSCLK_H
#define SYSCLK_H

#include <stdint.h>
#include "stm32l475xx.h"
#include "system.h"

void sysClkInit(void);
void system_clock_init_MSI(void);
void system_clock_init_HSI_80MHz(void);
void configure_pins(void);
void wait_ms_80MHz(uint32_t);
void wait_ms_timer(uint32_t);
void wait_us_80MHz(uint32_t);

void initTimer6TriggerDMA(uint32_t usecWait);


#endif
