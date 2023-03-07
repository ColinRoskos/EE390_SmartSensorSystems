#ifndef SYSTIME_H
#define SYSTIME_H

// libraries
#include "stdint.h"
#include "stdlib.h"
#include "sysclk.h"

// local dependencies

// local constants

#define SYSTIME_REQUEST_INPUT "\n\rPlease enter sytem time (HH:MM:SS): \0"
#define SYSTIME_INVALID_FORMAT 	"\n\r---- Invalid input passed, please enter in HH:MM:SS format ----\0"
#define SYSTIME_INVALID_TIME 	"\n\r---- Invalid input passed, please enter a valid time ----\0"
#define SYSTIME_INVALID_CHARACTER "\n\r---- Invalid input passed, please enter only numbers and ':' ----\0"
#define SYSTIME_SUCSESS "\n\rSystem time has been successfully set\0"
#define SYSTIME_OUTPUT_PREFIX "\n\rCurrent time is \0"//"\n\rSystem time is: \0"
#define SYSTIME_BUFFEREND_NEWLINECR "\n\r\0"

#define SYSTIME_TIME_FORMAT_WIDTH 8

// data

static struct{
	uint8_t hrs;
	uint8_t min;
	uint8_t sec;	
} systemTime;

// prototype functions
void setSysTime(void);
char* processMsg(char* pmsg);
void printSystemTime(void);
void displaySystemTime(void);
void rePrintSystemTime(void);
void secTick(void);

#endif
