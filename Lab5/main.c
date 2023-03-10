/*
*  Lab 5 | I2C Interfacing & relative humidity measurement | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
 * Configure the I2C2 module properly (setting the frequency of I2C to either 
 *  48-MHz or 80 MHz). Use the interrupt-driven approach to access the I2C 
 *  peripherals on the STM32L475 Discovery IoT kit (B-L475E-IOT01A). Use the 
 *  HTS221 sensor to measure the relative humidity and temperature. 
 *
*/

#include "main.h"

static char usartBuf[100];

int main(void)
{
  double humidity, temp;
  
	sysClkInit();
	initUSART1();
	initI2C();
	initHTS221( 0x1B, 0x84, 0x00, 0x00 );
	
  putsUSART1("AFTER INIT\n\r");
  
	for(;;)
	{
    putsUSART1("ONE_SHOT req\n\r");
		HTS221oneShotMeasure();
    putsUSART1("Wait req\n\r");
    HTS221waitforHTMeasurement();
    humidity = HTS221getHumidity();
    temp = HTS221getTemp();
    
    sendTHtoUSART(temp, humidity);
    
    wait_ms_80MHz(1000);
	}
	
	return 1;
}

void sendTHtoUSART(double temp, double hum)
{
  	putsUSART1(CLEAR_LINE);
		putsUSART1(UP_LINE);
    sprintf(usartBuf, MESSAGE_SPI_CONVERSION, hum, temp);
    putsUSART1(usartBuf);
    putcUSART1(ASCII_DEGREE);
    putsUSART1(MESSAGE_DEG);
}
