/*
 * timerMngr.c
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */


#include "string.h"
#include "xtmrctr.h"
#include "timerMngr.h"

#define TIMER_PERIOD_NS	10

// Timer
#define TIMER_CNTR_0	0
XTmrCtr xtimer[NB_PROCESSORS];
XTmrCtr* TmrCtrInstancePtr[NB_PROCESSORS];


void initTimers(){
	int i, Status;
	// Initializing handlers.
	for (i = 0; i < NB_PROCESSORS; i++) {
		TmrCtrInstancePtr[i] = &xtimer[i];
	}
	// Initialize device and link it to a handler. Ok if the device was already initialized by the µblaze.
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr[0], XPAR_TIMERS_AXI_TIMER_0_DEVICE_ID);
	if ((Status != XST_SUCCESS) || (Status != XST_DEVICE_IS_STARTED)) {
//		TODO: ..handle error
	}
	// Reset timer
	XTmrCtr_SetResetValue(TmrCtrInstancePtr[0], TIMER_CNTR_0, 0);
	XTmrCtr_SetOptions(TmrCtrInstancePtr[0], TIMER_CNTR_0,
						XTC_AUTO_RELOAD_OPTION |
						XTC_CASCADE_MODE_OPTION);
//	XTmrCtr_Reset(TmrCtrInstancePtr[0], TIMER_CNTR_0);
	XTmrCtr_Start(TmrCtrInstancePtr[0], TIMER_CNTR_0);

}


void fillTimeData(u64 cycles, TIME_DATA* ptr){
	u64 nanoseconds = cycles * TIMER_PERIOD_NS; // Converts to nanoseconds
	ptr->seconds = (nanoseconds / 1000000000); // Divide by 10^9 to get seconds

	u64 rest = nanoseconds % 1000000000;
	ptr->miliseconds = (rest / 1000000); // Divide by 10^6 to get ms

	rest = rest % 1000000;
	ptr->microseconds = (rest / 1000);
}


void printExecTimes(u32 nbProcessors, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1]){
	u8 i, j;
	TIME_DATA tdata;
	for (i = 0; i < nbProcessors; i++) {
		for (j = 0; j < NB_ACTORS; j++) {
			fillTimeData(execTimes[i][j], &tdata);
			xil_printf("proc %d actor %d : %d s ; %d ms ; %d µs\n\r", i, j, tdata.seconds, tdata.miliseconds, tdata.microseconds);
		}
		fillTimeData(execTimes[i][NB_ACTORS], &tdata);
		xil_printf("proc %d total time : %d s ; %d ms ; %d µs\n\r", i, tdata.seconds, tdata.miliseconds, tdata.microseconds);
	}
}

void printFPS(u32 nbFrames, u64 totalTime){
	TIME_DATA tdata;
	u32 fps = 0;
	fillTimeData(totalTime, &tdata);
	if(tdata.seconds > 0){
		fps = nbFrames / tdata.seconds;
		xil_printf("%d frames per seconds \n\r", fps);
	}
}
