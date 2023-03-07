
#ifndef SYSCLK_H
#define SYSCLK_H

#include "stm32l475xx.h"
#include <stdint.h>

void sysClkInit(void);
void system_clock_init_MSI(void);
void system_clock_init_HSI_80MHz(void);
void configure_pins(void);
void wait_ms_80MHz(uint32_t);
void wait_ms_timer(uint32_t);
void wait_us_80MHz(uint32_t);


#endif
