

#include "usart.h"

void initUART ()
{
	RCC->AHB2ENR |= RCC_AHB2SMENR_GPIOBSMEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
// Configure PB6 & PB7 for alternate function
	GPIOB->MODER &= ~(GPIO_MODER_MODE6 + GPIO_MODER_MODE7); // clear bits bit 12~15
	GPIOB->MODER |= GPIO_MODER_MODE6_1 + GPIO_MODER_MODE7_1; 
	
// Assign PB6 & PB7 pins to alternate function 7
	GPIOB->AFR[0] 	&= ~(GPIO_AFRL_AFSEL6 + GPIO_AFRL_AFSEL7); // clear bits 24~31
	GPIOB->AFR[0] 	|= (GPIO_AFRL_AFSEL6_2 + GPIO_AFRL_AFSEL6_1 + GPIO_AFRL_AFSEL6_0 + \
										GPIO_AFRL_AFSEL7_2 + GPIO_AFRL_AFSEL7_1 + GPIO_AFRL_AFSEL7_0);
	
	USART1->CR1 	&= ~(USART_CR1_M1 + USART_CR1_M0 + USART_CR1_OVER8); // select 8 data bits
	USART1->BRR 	= 0x2B6; // set baud rate to 115,200 baud
	USART1->CR2 	&= ~(USART_CR2_STOP); // select one stop bit
	USART1->CR3		&= ~(USART_CR3_ONEBIT); // use 3 samples to detect start bit, determine bit value
// enable USART1, enable transmitter, receiver	
	USART1->CR1 	|= USART_CR1_UE + USART_CR1_TE + USART_CR1_RE; 
}


void putsUART(char* pmsg)
{

	while(*pmsg){
			
		putcUART(*pmsg++);
		
	}
}	

void putcUART(char msgChar)
{
	
	while(!(USART1->ISR & USART_ISR_TXE)); 
	USART1->TDR = (uint16_t)msgChar; 

}

void getsUART(char* msg)
{
	
	char	cx;
	
	while ((cx = getcUART()) != ENTER) { // received an ENTER character?
		*msg = cx;
		putcUART(cx);	// echo the received character
		if (cx == BS) { // received a backspace character?
			_bsUART(); // backpace
			msg--;
		}
		else msg++;
	}
	*msg = 0;		// terminate the string with a NULL character


	return;
}

char getcUART()
{
	
	while (!(USART1->ISR & USART_ISR_RXNE));
	
	return ((char)USART1->RDR);

}

void putNewLineUART()
{
	putcUART(ASCII_CR);
	putcUART(ASCII_LF);
}

void _bsUART(void)
{
	putcUART(SPACE); // clear the character to be backspaced
	putcUART(BS); // move cursor backward 
}

void backspaceUART(uint16_t num)
{
	for(uint16_t i = 0 ; i < num ; i++)
	{
		//putcUART(SPACE); // clear the character to be backspaced
		putcUART(BS); // move cursor backward 
	}
}
