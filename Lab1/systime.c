/*
* systime.c
* a helper library for use in Lab 1 of EE390 - Smart Sensor Systems
* 
* Author : Colin Roskos
*/



#include "systime.h"
#include "usart.h"
#include "system.h"

void setSysTime(){
	char* msgBuf;
	
	msgBuf = (char*) malloc(sizeof(char) * 128);
	
	putsUART(SYSTIME_REQUEST_INPUT);
	
	getsUART(msgBuf);
	
	putsUART(processMsg(msgBuf)); // process message and respond feedback to the user
	
	free(msgBuf);
}

char* processMsg(char* pmsg)
{
	uint8_t numPort = 0; // numeric portion for input processing
	
	// buffers for System Time input checking
	uint8_t hrs = 0; 
	uint8_t min = 0;
	uint8_t sec = 0;
	
	// start input check with fisrt character in pmsg (char at index 0)
	uint8_t i = 0;
	char msgChar = pmsg[i];
	
	while(i < SYSTIME_TIME_FORMAT_WIDTH && msgChar)
	{
		// check that characters passed are valid
		if 				(msgChar > ASCII_COLON) return SYSTIME_INVALID_CHARACTER;
		else if 	(msgChar < ASCII_ZERO) return SYSTIME_INVALID_CHARACTER;
		else{} // rest of code in loop
		
		// an ASCII number is 0x30 <= Num < 0x3A, the 0x30 case is taken care of
		if (msgChar != ASCII_COLON)
		{
			if(numPort++ > 1) return SYSTIME_INVALID_FORMAT;
			
			if(i < 2) // if in hours section
			{
				hrs = hrs * 10; // if second digit, shift existing digit to tens place
				hrs += msgChar - ASCII_ZERO; // assign ones place
			}
			else if(i < 5) // if in minutes seciton
			{
				min = min * 10; // if second digit, shift existing digit to tens place
				min += msgChar - ASCII_ZERO; // assign ones place
			}
			else // in seconds section
			{
				sec = sec * 10; // if second digit, shift existing digit to tens place
				sec += msgChar - ASCII_ZERO; // assign ones place
			}	
		}
		// if msg char is ":" it must have exactly 2 numbers preceding it
		else
		{
			if (numPort != 2) return SYSTIME_INVALID_FORMAT;
			
			numPort = 0;
		}
		
		// incrementors
		msgChar = pmsg[++i];
	}
	
	// check time
	if (hrs > 23 || min > 60 || sec > 60) return SYSTIME_INVALID_TIME;
	
	// set valid time to systemTime
	systemTime.hrs = hrs;
	systemTime.min = min;
	systemTime.sec = sec;
	
	return SYSTIME_SUCSESS;
}

void printSystemTime(void)
{
		
	putsUART(SYSTIME_OUTPUT_PREFIX);
	
	displaySystemTime();
	
}

void displaySystemTime(void)
{
	char * msgBuf;
	
	msgBuf = (char*) malloc(sizeof(char) * 9);
	
	msgBuf[0] = uint2char(systemTime.hrs/10); //10s place
	msgBuf[1] = uint2char(systemTime.hrs);
	msgBuf[2] = ASCII_COLON;
	msgBuf[3] = uint2char(systemTime.min/10); //10s place
	msgBuf[4] = uint2char(systemTime.min);
	msgBuf[5] = ASCII_COLON;
	msgBuf[6] = uint2char(systemTime.sec/10); //10s place
	msgBuf[7] = uint2char(systemTime.sec);
	msgBuf[8] = ASCII_EOL;
	
	putsUART(msgBuf);
	
	free(msgBuf);
}

void rePrintSystemTime(void)
{
	backspaceUART(8); // backup output by 8 chars
	displaySystemTime(); // print system time HH:MM:SS
}

void secTick(void)
{
	wait_ms_80MHz(1000);
	systemTime.sec++;
	if(systemTime.sec == 60)
	{
		systemTime.sec = 0;
		systemTime.min++;
	}
	if(systemTime.min == 60)
	{
		systemTime.min = 0;
		systemTime.hrs++;
	}
	if(systemTime.hrs == 24)
	{
		systemTime.hrs = 0;
	}
}
