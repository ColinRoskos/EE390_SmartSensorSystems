

#include "usart.h"

void initUSART1 ()
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

uint32_t pollUSART1(char * pMsgBuff)
{
	if (!(USART1->ISR & USART_ISR_RXNE)) return 0;
	
	getsUSART1(pMsgBuff);
	return 1;
}

void putsUSART1(char* pmsg)
{

	while(*pmsg){
			
		putcUSART1(*pmsg++);
		
	}
}	

void puthUSART1(uint16_t val)
{
	uint16_t inter = val;
	char chars[4] = {0};
	for(uint8_t i = 0; i < 4; i++)
	{
		chars[3 - i] = (hex2char((uint8_t) inter & 0xf));
		inter = inter >> 4;
	}
	
	for(uint8_t i = 0; i < 4; i++)
	{
		putcUSART1(chars[i]);
	}
}

void putuUSART1(uint32_t val)
{
	char msg[10];
	uint32_t inter = val;
	uint16_t index = 0;
	
	// if 0 is the value passed
	if(!val){
		putcUSART1(ASCII_ZERO);
		return;
	}
	
	while(inter)
	{
		msg[index++] = uint2char(inter % 10);
		inter = inter/10;
	}
	while(index)
	{
		putcUSART1(msg[--index]);
	}
}

void putcUSART1(char msgChar)
{
	
	while(!(USART1->ISR & USART_ISR_TXE)); 
	USART1->TDR = (uint16_t)msgChar; 

}


void getsUSART1(char* msg)
{
	
	char	cx;
	
	while ((cx = getcUSART1()) != ENTER) { // received an ENTER character?
		*msg = cx;
		putcUSART1(cx);	// echo the received character
		if (cx == BS) { // received a backspace character?
			_bsUSART1(); // backpace
			msg--;
		}
		else msg++;
	}
	*msg = 0;		// terminate the string with a NULL character


	return;
}

char getcUSART1()
{
	
	while (!(USART1->ISR & USART_ISR_RXNE));
	
	return ((char)USART1->RDR);

}

void putNewLineUSART1()
{
	putcUSART1(ASCII_CR);
	putcUSART1(ASCII_LF);
}

void _bsUSART1(void)
{
	putcUSART1(SPACE); // clear the character to be backspaced
	putcUSART1(BS); // move cursor backward 
}

void backspaceUSART1(uint16_t num)
{
	for(uint16_t i = 0 ; i < num ; i++)
	{
		putcUSART1(BS); // move cursor backward 
	}
}


void printMemory(uint32_t * mem, uint32_t pos)
{
	putsUSART1("\n\r");
	
	char * asciiStr = malloc(sizeof(char) * 16);
	uint8_t asciii = 0;
	uint32_t temp;
	
	for(uint32_t ipos = 0; ipos < pos ; ipos++)
	{
		temp = mem[ipos];
		asciiStr[asciii + 3] = (uint8_t) temp & 0xff;
		temp = temp >> 8;
		asciiStr[asciii + 2] = (uint8_t) temp & 0xff;
		temp = temp >> 8;
		asciiStr[asciii + 1] = (uint8_t) temp & 0xff;
		temp = temp >> 8;
		asciiStr[asciii] = (uint8_t) temp & 0xff;
		
		puthUSART1((uint16_t) ((mem[ipos] >> 16) & 0xffff)); // print upper
		puthUSART1((uint16_t) mem[ipos] & 0xffff);
		putcUSART1(ASCII_SP);
		
		asciii++;
		if (asciii == 4){
			asciii = 0;
			_printMemoryStrPortion(asciiStr);
		}
	}
	
	if((pos % 4) != 0)
	{
		for(uint8_t i = asciii; i < 4 ; i++)
		{
			putsUSART1("XXXXXXXX");
			putcUSART1(ASCII_SP);
			asciiStr[asciii] = ASCII_SP;
			asciiStr[asciii + 1] = ASCII_SP;
			asciiStr[asciii + 2] = ASCII_SP;
			asciiStr[asciii + 3] = ASCII_SP;
		}
		_printMemoryStrPortion(asciiStr);
	}
	
	free(asciiStr);
}

void _printMemoryStrPortion(char * str)
{
	for(uint8_t asi = 0; asi < 16 ; asi++)
	{
		if(str[asi] < ASCII_SP || str[asi] == ASCII_DEL || str[asi] == 0xff)
		{
			str[asi] = ASCII_SP;
		}
	}
	putcUSART1(0x7C);
	putcUSART1(ASCII_SP);
	putsUSART1(str);
	putsUSART1("\n\r");
}


void insertsUSART1(char * fmt, char * str)
{	
	char ch;
	while( (ch = *fmt++) )
	{
		if( ch != FORMAT_DELIMITTER)
		{
			putcUSART1(ch);
		}
		else
		{
			switch(ch = *fmt++){
				case 's':
					putsUSART1(str);
					break;
				default:
					putcUSART1(ch);
					break;
			}
		}
	}
}
