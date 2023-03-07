
#include "sysclk.h"

void sysClkInit()
{
	system_clock_init_HSI_80MHz();
}


void wait_ms_80MHz(uint32_t time_ms){
	for(uint32_t delay = 0; delay < time_ms ; delay++){
/*	 SUBS - 1 cycle, NOP - 1 cycle, BNE - 2 cycles ,
~ 		4 cycles total per loop - 80MHz/4/20,000 = 1000Hz => 1ms 
*/
		for(uint32_t ms = 0; ms < 20000; ms++){
			/* when left to compiler on -O1 without asm, this loop is removed */
			/* when left to compiler on -O0 without asm, this loop is 15~20 instructions - I didn't care to count*/
			__asm("NOP"); 
		}
	}
}

void wait_ms_timer(uint32_t time_ms){
	/* use TIM7 (master) to on a div of 1 count up to 8,000 to give 1 ms into TIM6 (slave) */
	/* using clock input from APB1, set TIM6 to be the ms wanted */
	
	if((RCC->APB1ENR1 & RCC_APB1ENR1_TIM7EN) == 0){
		RCC->APB1ENR1|= RCC_APB1ENR1_TIM7EN; // enable APB1 for TIM7
	}
	
	TIM7->PSC = 79;
	TIM7->ARR = 1000; // 1000 - appx. time to set TIM7 reg
	TIM7->CNT = 0;
	TIM7->EGR = TIM_EGR_UG;
	TIM7->SR 	= 0; // reset UIF bit
	TIM7->CR1 |= TIM_CR1_ARPE + TIM_CR1_CEN;
	
	for(uint32_t wait_ms = 0 ; wait_ms < time_ms ; wait_ms++){
		while(!(TIM7->SR & TIM_SR_UIF));
		TIM7->SR 	= 0; // reset UIF bit
	}
}


void wait_us_80MHz(uint32_t time_us){
	for(uint32_t delay = 0; delay < time_us ; delay++){
/*	 SUBS - 1 cycle, NOP - 1 cycle, BNE - 2 cycles ,
~ 		4 cycles total per loop - 80MHz/4/20 = 1MHz => 1us 
*/
		for(uint32_t us = 0; us < 20; us++){
			__asm("NOP");
		}
	}
}

void system_clock_init_MSI(void){
	RCC->CR |= RCC_CR_MSION;
	RCC->CFGR &= RCC_CFGR_SW;
	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS; /* Frequency is above 64MHz, 4 WS required */
	FLASH->ACR |= FLASH_ACR_PRFTEN;  /* prefetch enable */
	
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_6;
	
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0);
	SystemCoreClockUpdate();
	SPI1->CR1;
}

void system_clock_init_HSI_80MHz(void)
{
	//system_clock_init_MSI(); // cannot set HSI until it is not used as clock.
	
  // Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);

  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );
	
	PWR->CR1 &= ~PWR_CR1_VOS;
	PWR->CR1 |= PWR_CR1_VOS_0;
	while(PWR->SR2 & PWR_SR2_VOSF);
	
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS; /* Frequency is above 64MHz, 4 WS required */
	FLASH->ACR |= FLASH_ACR_PRFTEN;  /* prefetch enable */
	
  RCC->CR &= ~(RCC_CR_PLLON);
	while(RCC->CR & RCC_CR_PLLRDY)
		;

  // choose HSI as PLL Input
  RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSI + RCC_PLLCFGR_PLLN_4 +
                    RCC_PLLCFGR_PLLN_2 + RCC_PLLCFGR_PLLR_0);

  // Wait till PLL is used as system clock source
  RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		;
	RCC->PLLCFGR = RCC_PLLCFGR_PLLREN;
	
  // Select PLL as system clock source
  RCC->CFGR &= ~(RCC_CFGR_SW + RCC_CFGR_HPRE);
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL + RCC_CFGR_HPRE_1;  //11: PLL oscillator used as system clock, divide clock by 1
	
	while ((RCC->CFGR & (RCC_CFGR_SW_PLL + RCC_CFGR_HPRE_1)) == 0 );  //11: PLL oscillator used as system clock, divide clock by 1
	
	SystemCoreClockUpdate();
}

void initTimer6TriggerDMA(uint32_t usecWait)
{

	if((RCC->APB1ENR1 & RCC_APB1ENR1_TIM6EN) == 0){
		RCC->APB1ENR1|= RCC_APB1ENR1_TIM6EN; // enable APB1 for TIM6
		NOP; // delay one clock cycle to keep RCC change
	}
	
	TIM6->PSC = 7; // prescale 8
	TIM6->ARR = usecWait * 10; // wait usecWait us
	TIM6->CNT = 0;
	TIM6->DIER = TIM_DIER_UIE;
	
	TIM6->CR2 &= ~TIM_CR2_MMS_Msk;
	TIM6->CR2 |= TIM_CR2_MMS_1;
	
	TIM6->CR1 |= TIM_CR1_CEN;
}
