
#include "HTS221.h"

/*
 * initHTS221
 * must initialize i2c before use. Sets up HTS221 Humidity and Temperature 
 *   sensor for use.
 *
 * input:
 *   avConf : (uint8_t) Configuration value for the number of samples to be 
 *     averaged. (see Table 1) (reset 0x1B)
 *   ctrl1 :  (uint8_t) Control register 1. (see Table 2) (reset 0x00)
 *   ctrl2 :  (uint8_t) Control register 2. (see Table 3) (reset 0x00)
 *   ctrl3 :  (uint8_t) Control register 3. (see Table 4) (reset 0x00)
 * output:
 *   None : (void)
 * 
 *
 * Table 1: Control Register 1 (0x20)
 *   |  7  |  6  |   5   |   4   |   3   |   2   |   1   |   0   |
 *   | --- | --- | AVGT2 | AVGT1 | AVGT0 | AVGH2 | AVGH1 | AVGH0 |
 * 
 *         | Num Samples |
 *   | AVG | TEMP | HUMD |
 *   | 000 |   2  |   4  |
 *   | 001 |   4  |   8  |
 *   | 010 |   8  |  16  |
 *   | 011 |  16  |  32  |
 *   | 100 |  32  |  64  |
 *   | 101 |  64  | 128  |
 *   | 110 | 128  | 256  |
 *   | 111 | 256  |   4  |
 * 
 * Table 2: Control Register 1 (0x20)
 *   |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *   | PD  | --- | --- | --- | --- | BDU | ODR1| ODR2|
 * 
 *   PD : Power Down control
 *       0 : Power down mode
 *       1 : Active mode
 *   BDU : Block data update
 *       0 : Continuous mode. Lower and upper register parts are updated cont.
 *       1 : Single update mode. output registers not updated until both are read
 *   ODR : Output data rate selection
 *       00 : One-shot
 *       01 : Humidity/Temp  1 Hz
 *       10 : Humidity/Temp  7 Hz
 *       11 : Humidity/Temp 12.5 Hz
 *
 *
 * Table 3: Control Register 2 (0x21)
 *   |  7   |  6  |  5  |  4  |  3  |  2  |   1    |    0     |
 *   | BOOT | --- | --- | --- | --- | --- | Heater | One_Shot |
 * 
 *   BOOT : Reboot memory contents
 *       0 : Normal mode
 *       1 : Reboot memory contents
 *      At device power-up, the contents of the flash memory block is 
 *        transferred to internal registers related to trimming functions to 
 *        permit good behavior of the device. If, for any reason, the contents 
 *        of the trimming registers are modified, it is sufficient to set this 
 *        bit to restore the correct value (value stored in flash memory)
 *
 *   Heater : Block data update
 *       0 : Heater disabled
 *       1 : Heater enabled
 *      This bit controls internal heating element. This can speed up the
 *        sensor recovery time in the event of condensation.
 *    >> DO NOT READ TEMP/HUMID WHILE HEATER ENABLED << (this gives bad values)
 *
 *   One_Shot : Output data rate selection
 *       0 : Waiting for the start of conversion
 *       1 : Start of a new dataset
 *      When One_Shot bit is set, the chicp performs a single aquisition of temp
 *        and humidity. The conversion places the data into the output registers
 *        when available.
 *
 *
 * Table 4: Control Register 3 (0x22)
 *   |    7     |   6   |  5  |  4  |  3  |    2    |  1  |  0  |
 *   | DRDY_H_L | PP_OD | --- | --- | --- | DRDY_EN | --- | --- |
 * 
 *   DRDY_H_L : Hata ready output signal active high, low
 *       0 : Active high (default)
 *       1 : Active low
 *   PP_OD : Push-pull/Open-drain selection on pin3 (DRDY)
 *       0 : Push-pull (default)
 *       1 : Open-drain
 *   DRDY_EN : Output data rate selection
 *       0 : Data ready disabled (default)
 *       1 : Data ready signal available on pin 3 (DRDY)
 *      DRDY is a logical OR (STATUS_REG[0] | STATUS_REG[1]) of temp and 
 *        humidity, respectively.
 *
*/
void initHTS221(uint8_t avConf, uint8_t ctrl1, uint8_t ctrl2, uint8_t ctrl3)
{
	blkSize = wrBytes = 2;
	rdBytes = 0;
	wrNotDone = false;
	
	outx = 0;
	outBuf[0] = HTS221_ADDR_AV_CONF; // address of AV_CONF register
	outBuf[1] = avConf; // set sample rate
	
	I2C2->CR2 = I2C_CR2_AUTOEND + (2 << I2C_CR2_NBYTES_Pos) + (HTS221_ADDRESS << 1); 
  NOP;
	I2C2->CR2 |= I2C_CR2_START; // generate a START condition
	
	while(wrBytes){NOP;}
	
	outBuf[0] = HTS221_ADDR_CTRL_REG1; // address of CTRL_REG1 (0x20) + bit 7 set to 1
	outBuf[1] = ctrl1;
	outBuf[2] = ctrl2;
	outBuf[3] = ctrl3;
	
	blkSize = wrBytes = 4;
	outx = 0;
	
	I2C2->CR2	= I2C_CR2_AUTOEND + (4 << I2C_CR2_NBYTES_Pos) + (HTS221_ADDRESS << 1);
	I2C2->CR2 |= I2C_CR2_START; // generate a START condition
	
	while(wrBytes){NOP;}
}

void readHTS221Regs (uint16_t cnt, uint8_t addr)
{
	wrBytes = 1;	// register address to be sent
	blkSize = 1;
	rdBytes = cnt;
	wrNotDone = 0;
	outx = 0;
	inx	= 0;
	if (cnt > 1)
		outBuf[0] 	= addr + 0x80; // prepare to read multiple registers
	else
		outBuf[0] 	= addr; 
	I2C2->CR2 	= I2C_CR2_AUTOEND + (1 << I2C_CR2_NBYTES_Pos) + 0xBE;  //101 1111
	I2C2->CR2 	|= I2C_CR2_START;
	while(rdBytes){NOP;}
}

void HTS221oneShotMeasure(void)
{
	wrBytes = blkSize = 2;
	rdBytes = 0;
	wrNotDone = 0;
	outx = 0;
	outBuf[0] = 0x21; // address of CTRL_REG2
	outBuf[1] = 0x01; // command to start one-shot measurement
	I2C2->CR2 = I2C_CR2_AUTOEND + (2 << I2C_CR2_NBYTES_Pos) + 0xBE; 
	I2C2->CR2 |= I2C_CR2_START;
	
	while(wrBytes){NOP;}
}

void HTS221waitforHTMeasurement (void)
{
	uint32_t notDone;
	notDone = 1;
	while(notDone) {
	  readHTS221Regs(1, 0x27);  // read STATUS_REG
	  if((inBuf[0] & 0x03) == 0x03)  // check T_DA & H_DA bits
	    notDone = 0;
	}
}

double HTS221getHumidity(void)
{
	double humidity;
	
	uint8_t humH, humL;
	int16_t hOut;
	
	uint8_t h0RH, h1RH;
	uint8_t h0ToutL, h0ToutH, h1ToutL, h1ToutH;
	int16_t h0Tout, h1Tout;
  
  static bool hVarsInit = false;
  
  static double dh0Tout, dh0RH = 0.0; // used in future computation of temp
	static double rhPerc = 0.0; // static variable to be setup at startup
	
	if(!hVarsInit)
	{
		readHTS221Regs(2, HTS221_ADDR_CAL_H0_RH); // gives HTS221_ADDR_CAL_H0_RH and HTS221_ADDR_CAL_H1_RH
		h0RH = inBuf[0] >> 1; // divide by 2
		h1RH = inBuf[1] >> 1;
    
		readHTS221Regs(2, HTS221_ADDR_CAL_H0_T0_OUT); // gives HTS221_ADDR_H0_T0_OUT
		h0ToutL = inBuf[0];
		h0ToutH = inBuf[1];
    
		readHTS221Regs(2, HTS221_ADDR_CAL_H1_T0_OUT); // gives HTS221_ADDR_H1_T0_OUT
		h1ToutL = inBuf[0];
		h1ToutH = inBuf[1];
		
		h0Tout = (int16_t) (h0ToutH << 8) + h0ToutL;
		h1Tout = (int16_t) (h1ToutH << 8) + h1ToutL;
    
    dh0Tout = (double) h0Tout;
    dh0RH = (double) h0RH;
    
    rhPerc = (((double) h1RH) - dh0RH)/(((double) h1Tout) - dh0Tout);
    hVarsInit = true;
	}
	
	readHTS221Regs(2, HTS221_ADDR_HUMID_OUT_L); // gives HTS221_ADDR_HUMID_OUT
	humL = inBuf[0];
	humH = inBuf[1];
	hOut = (int16_t) (humH << 8) + humL;
	
	humidity = rhPerc * (((double) hOut) - dh0Tout) + dh0RH;
	
	return humidity;
}

double HTS221getTemp(void)
{
	double temp;
	
	uint8_t tempH, tempL;
	uint16_t t0DegC, t1DegC; 
  int16_t t0out, t1out;
  int16_t tOut;
  
  
  static bool tVarsInit = false;
  
  static double dt0DegC, dt0out = 0.0; // used in future computation of temp
	static double rTemp = 0.0; // static variable to be setup at startup, prevents recalculation
	
  if (!tVarsInit)
  {
    readHTS221Regs(2, HTS221_ADDR_CAL_T0_OUT); // gives HTS221_ADDR_CAL_T0_OUT lsb first
		t0out = (int16_t) (inBuf[1] << 8) + inBuf[0];
    
    readHTS221Regs(2, HTS221_ADDR_CAL_T1_OUT); // gives HTS221_ADDR_CAL_T1_OUT lsb first
		t1out = (int16_t) (inBuf[1] << 8) + inBuf[0];
    
		readHTS221Regs(3, HTS221_ADDR_CAL_DEGC); // gives T0_degC, T1_degC, T1/T0 msb
		t0DegC = (uint16_t) ((inBuf[2] & HTS221_ADDR_CAL_T0_MSB_Msk) << 8) + inBuf[0];
		t1DegC = (uint16_t) ((inBuf[2] & HTS221_ADDR_CAL_T1_MSB_Msk) << 6) + inBuf[1];

    t0DegC = t0DegC >> 3; // divide by 8
    t1DegC = t0DegC >> 3;
    
    dt0DegC = (double) t0DegC;
    dt0out = (double) t0out;
    
    rTemp = (((double) t1DegC) - dt0DegC) / (((double) t1out) - dt0out);
    tVarsInit = true;
  }
  
  readHTS221Regs(2, HTS221_ADDR_TEMP_OUT_L); // gives HTS221_ADDR_HUMID_OUT
	tempL = inBuf[0];
	tempH = inBuf[1];
	tOut = (int16_t) (tempH << 8) + tempL;
  
  temp = (rTemp * (((double) tOut) - dt0out)) + dt0DegC; 
  
	return temp;
}
bool HTS221getID(void)
{
	readHTS221Regs(1, HTS221_ADDR_WHO_AM_I); // put WHO_AM_I into inBuf[0]
	return (HTS221_EXPECT_WHO_AM_I == inBuf[0]);
}
