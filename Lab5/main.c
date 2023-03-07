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

int main(void)
{
	sysClkInit();
	initUSART1();
	
}
