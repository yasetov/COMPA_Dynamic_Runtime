/*
 * timeMngr.c
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#include "timerMngr.h"

#define TIMER_PERIOD_NS	10

extern void* TmrCtrInstancePtr;
extern u32 XTmrCtr_GetValue();

u64 getTimerValue64(){
	u64 result = 0;
	u32 lower, upper = -1;

	while(upper != result){
		upper = XTmrCtr_GetValue(TmrCtrInstancePtr, 1);
		lower = XTmrCtr_GetValue(TmrCtrInstancePtr, 0);
		result = XTmrCtr_GetValue(TmrCtrInstancePtr, 1);
	}
	result = result << 32;
	result += lower;

	return result;
}

void fillTimeData(u64 cycles, TIME_DATA* ptr){
	u64 nanoseconds = cycles * TIMER_PERIOD_NS; // Converts to nanoseconds
	ptr->seconds += (nanoseconds / 1000000000); // Divide by 10^9 to get seconds

	u64 rest = nanoseconds % 1000000000;
	ptr->miliseconds += (rest / 1000000); // Divide by 10^6 to get ms

	rest = rest % 1000000;
	ptr->microseconds += (rest / 1000);
}

