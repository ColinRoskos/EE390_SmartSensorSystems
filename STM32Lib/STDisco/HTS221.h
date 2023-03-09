/* 
 * Header for use with ST HTS221 Temperature and Humidity Sensor
 * Author Colin Roskos 
*/

#ifndef HTS221_H
#define HTS221_H


#include "stm32l475xx.h"
#include "../i2c.h"

#define HTS221_ADDRESS 0x5F

// HTS221 address list
// data registers
#define HTS221_ADDR_WHO_AM_I 0x0F
#define HTS221_EXPECT_WHO_AM_I 0xBC
#define HTS221_ADDR_AV_CONF 0x10
#define HTS221_ADDR_CTRL_REG1 0x20
#define HTS221_ADDR_CTRL_REG2 0x21
#define HTS221_ADDR_CTRL_REG3 0x22
#define HTS221_ADDR_STATUS_REG 0x27
#define HTS221_ADDR_HUMID_OUT_L 0x28
#define HTS221_ADDR_HUMID_OUT_H 0x29
#define HTS221_ADDR_TEMP_OUT_L 0x2A
#define HTS221_ADDR_TEMP_OUT_H 0x2B

// calibration registers
#define HTS221_ADDR_CALIBRATION_START 0x30
#define HTS221_ADDR_CALIBRATION_END 0x3F
//   Humidity Calibration address
#define HTS221_ADDR_CAL_H0_RH 0x30
#define HTS221_ADDR_CAL_H1_RH 0x31
#define HTS221_ADDR_CAL_H0_T0_OUT 0x36
#define HTS221_ADDR_CAL_H0_T0_OUT_0 HTS221_ADDR_CAL_H0_T0_OUT
#define HTS221_ADDR_CAL_H0_T0_OUT_1 0x37
#define HTS221_ADDR_CAL_H1_T0_OUT 0x3A
#define HTS221_ADDR_CAL_H1_T0_OUT_0 HTS221_ADDR_CAL_H1_T0_OUT
#define HTS221_ADDR_CAL_H1_T0_OUT_1 0x3B
//    Temperature Calibration address
#define HTS221_ADDR_CAL_DEGC 0x32
#define HTS221_ADDR_CAL_T0_DEGC HTS221_ADDR_CAL_DEGC
#define HTS221_ADDR_CAL_T1_DEGC 0x33
#define HTS221_ADDR_CAL_T_MSB 0x33
#define HTS221_ADDR_CAL_T0_MSB_Msk 0x03
#define HTS221_ADDR_CAL_T1_MSB_Msk 0x0C
#define HTS221_ADDR_CAL_T0_OUT 0x3C
#define HTS221_ADDR_CAL_T1_OUT 0x3E

void initHTS221(uint8_t, uint8_t, uint8_t, uint8_t);
void readHTS221Regs (uint16_t, uint8_t);
void HTS221oneShotMeasure(void);
void HTS221waitforHTMeasurement (void);
double HTS221getHumidity(void);
double HTS221getTemp(void);
bool HTS221getID(void);

#endif
