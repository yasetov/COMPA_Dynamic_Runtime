/*
 * timeMngr.h
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#ifndef TIME_MNGR_H_
#define TIME_MNGR_H_
#include "types.h"

typedef struct{
	u32 minutes;
	u32 seconds;
	u32 miliseconds;
	u32 microseconds;
}TIME_DATA;

void fillTimeData(u64 cycles, TIME_DATA* ptr);

#endif /* TIME_MNGR_H_ */
