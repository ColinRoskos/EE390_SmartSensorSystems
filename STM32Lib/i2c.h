/* 
 * I2C Header for use with STM32
 * Author Colin Roskos 
*/

#ifndef I2C_H
#define I2C_H

#include "stdbool.h"
#include "stm32l475xx.h"

#include "sysclk.h"
#include "system.h" 

// making the i2c buffer entities global
// TODO: make only the buffers accessable globally?
extern uint32_t blkSize, wrBytes, rdBytes;
extern uint8_t inBuf[200], outBuf[100];
extern uint16_t inx, outx;
extern bool wrNotDone, inNACK;

void I2C2_EV_IRQHandler(void);
void writePoll (uint8_t);
void i2cSetBlockSize(uint32_t);
void i2cSetWriteBytes(uint32_t);
uint32_t i2cGetWriteBytes(void);
void i2cSetReadBytes(uint32_t);
uint32_t i2cGetReadBytes(void);
void resetOutx(void);
void resetInx(void);
void initI2C(void);

#endif
