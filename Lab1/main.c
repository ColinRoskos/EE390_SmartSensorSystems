/*
 * Lab 1 | USART Set Clock | Colin Roskos
 * Minnesota State University, Mankato
 * EE390 Smart Sensor Systems
 * --------------------------------
 *
*/

#include "main.h"

int main(){
	sysClkInit();
	initUART();
	
	setSysTime();
		
	printSystemTime();
	
	for(;;)
	{
		secTick();
		rePrintSystemTime();
		
	}
	
	return SUCCESS;
}


