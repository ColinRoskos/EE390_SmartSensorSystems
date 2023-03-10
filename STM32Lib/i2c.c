#include "i2c.h"

uint32_t blkSize, wrBytes, rdBytes = 0;
uint8_t inBuf[200], outBuf[100] = {0};
uint16_t inx, outx = 0;
bool wrNotDone, inNACK = false;

void I2C2_EV_IRQHandler(void)
{
	if(I2C2->ISR & I2C_ISR_TXIS) { // if transmitting
		if (blkSize != 0) 
		{
			I2C2->TXDR = outBuf[outx++]; 
			wrBytes--;
			blkSize--;
		} 
		else 
		{
			if((I2C2->ISR & I2C_ISR_TC) != 0){ // start the read process
				I2C2->CR2 &= ~I2C_CR2_NBYTES;
				if (rdBytes < 256) 
				{
					I2C2->CR2 |= (rdBytes << I2C_CR2_NBYTES_Pos) + I2C_CR2_RD_WRN +
							 I2C_CR2_AUTOEND;
					blkSize = rdBytes;
				}
				else 
				{
					I2C2->CR2 |= (0xFF << I2C_CR2_NBYTES_Pos) + I2C_CR2_RD_WRN + 
							I2C_CR2_RELOAD;
					blkSize = 0xFF;
				}
				I2C2->CR2 |= I2C_CR2_START;
			}
		}
	}
	else if(I2C2->ISR & I2C_ISR_TCR) // reload for transmission
	{ 
		if (wrBytes < 256) {
			I2C2->CR2 &= ~I2C_CR2_NBYTES;
			I2C2->CR2 |= (wrBytes << I2C_CR2_NBYTES_Pos) + I2C_CR2_AUTOEND;
			blkSize = wrBytes;
		}
		else {
			I2C2->CR2 |= (0xFF << I2C_CR2_NBYTES_Pos) + I2C_CR2_RELOAD;
			blkSize = 0xFF;
		}
	}
	else if(I2C2->ISR & I2C_ISR_RXNE) // if recieving
	{
		inBuf[inx++] = (uint8_t)I2C2->RXDR; // store the received byte
		rdBytes--;
		blkSize--; 
		if(blkSize == 0) {
			if((I2C2->ISR & I2C_ISR_TCR) != 0){
				if (rdBytes < 256){
					blkSize = rdBytes; 
					I2C2->CR2 |= (rdBytes << I2C_CR2_NBYTES_Pos) + I2C_CR2_AUTOEND;
				}
				else {
					blkSize = 0xFF;
					I2C2->CR2 |= (0xFF << I2C_CR2_NBYTES_Pos) + I2C_CR2_RELOAD;
				}	
			}
		}
	}
	else if(I2C2->ISR & I2C_ISR_NACKF) 
	{
		I2C2->ICR = I2C_ICR_NACKCF; // clear NACK flag
		inNACK = true;
		I2C2->CR2 |= I2C_CR2_START; // repoll the EEPROM
	}
}


void i2cSetBlockSize(uint32_t size)
{
  blkSize = size;
}
 
void i2cSetWriteBytes(uint32_t size)
{
  wrBytes = size;
}

uint32_t i2cGetWriteBytes(void)
{
  return wrBytes;
}
 
void i2cSetReadBytes(uint32_t size)
{
  rdBytes = size;
}

uint32_t i2cGetReadBytes(void)
{
  return rdBytes;
}

void resetOutx(void)
{
  outx = 0;
}

void resetInx(void)
{
  inx = 0;
}


void writePoll (uint8_t SLAddr)
{
	wrNotDone = true; 
	inNACK = false;	// hasn’t received NACK yet
	I2C2->CR2 = (uint32_t) (SLAddr << 1);
	I2C2->CR2 |= I2C_CR2_START; // generate a START condition
	while(wrNotDone){ // test whether I2C2 has been NACK but now received ACK
		if(inNACK && ((I2C2->ISR & I2C_ISR_NACKF) == 0))
			break; 
	}
}


void initI2C(void)
{
  // enable clock for I2C
	RCC->AHB2ENR |= RCC_AHB2SMENR_GPIOBSMEN;
	RCC->APB1ENR1 = RCC_APB1ENR1_I2C2EN;
  
	// setup GPIOB pins 10, 11 for I2C
	// configure PB10~PB11 for alternate function
	GPIOB->MODER 	&= 	~(GPIO_MODER_MODE10 + GPIO_MODER_MODE11);
	GPIOB->MODER 	|= 	GPIO_MODER_MODE10_1 + GPIO_MODER_MODE11_1; 
	// assign PB10~PB11 to AF4
	GPIOB->AFR[1] 	&= 	~(GPIO_AFRH_AFSEL10 + GPIO_AFRH_AFSEL11);
	GPIOB->AFR[1] 	|= 	GPIO_AFRH_AFSEL10_2 + GPIO_AFRH_AFSEL11_2; 
	// configure PB10~PB11 to open drain
	GPIOB->OTYPER 	|= 	GPIO_OTYPER_OT10 + GPIO_OTYPER_OT11; 
	
	/* FOR USE WITH 48MHz
	if(fs == 0) // fI2CCLK = 48 MHz
		I2C2->TIMINGR = (5 << I2C_TIMINGR_PRESC_Pos) + 
			(3 << I2C_TIMINGR_SCLDEL_Pos) + (3 << I2C_TIMINGR_SDADEL_Pos) + 
			(3 << I2C_TIMINGR_SCLH_Pos) +
			(9 << I2C_TIMINGR_SCLL_Pos); // tSYNC1 + tSYNC2 = 250 ns
	else // fI2CCLK = 80 MHz (See Example 11.2)
	*/
	//  we are using 80MHz	
	I2C2->TIMINGR =  ((uint32_t) 9 << I2C_TIMINGR_PRESC_Pos) + // casting to uint32_t to avoid compiler warning
			(3 << I2C_TIMINGR_SCLDEL_Pos) + (2 << I2C_TIMINGR_SDADEL_Pos) + 
			(4 << I2C_TIMINGR_SCLH_Pos) +
			(10 << I2C_TIMINGR_SCLL_Pos); // tSYNC1 + tSYNC2 = 250 ns
			
// enable analog filter (ANOFF bit = 0), set DNF to 3, enable relevant interrupts and enable I2C2
	I2C2->CR1 	=  I2C_CR1_ERRIE + I2C_CR1_TCIE + I2C_CR1_NACKIE + (3 << I2C_CR1_DNF_Pos)
				 + I2C_CR1_TXIE + I2C_CR1_RXIE + I2C_CR1_PE;
	NVIC_SetPriority(I2C2_EV_IRQn, 0);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
	__enable_irq();
}

