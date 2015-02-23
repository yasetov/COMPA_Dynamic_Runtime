/*
 * timeMngr.h
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#ifndef TIME_MNGR_H_
#define TIME_MNGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "config.h"

typedef struct{
	u32 minutes;
	u32 seconds;
	u32 miliseconds;
	u32 microseconds;
}TIME_DATA;

void initTimers();
void fillTimeData(u64 cycles, TIME_DATA* ptr);
void printExecTimes(u32 nbProcessors, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1]);

#ifdef __cplusplus
}
#endif

#endif /* TIME_MNGR_H_ */
