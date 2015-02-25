/*
 * perfMonMngr.h
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#ifndef PERFMONMNGR_H_
#define PERFMONMNGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xaxipmon.h"

int initPerfMons();

void readPerfMonitor(XAxiPmon *AxiPmonInstPtr,
					u32 Metrics[],
					u32 *ClkCntHigh,
					u32 *ClkCntLow);

void printMetricValues(XAxiPmon *AxiPmonInstPtr, u8 nbSlots, u8 Metric);
void printWrittenBytesValues(u32 nbProcessors, u32 writtenBytes[NB_PROCESSORS][NB_ACTORS + 1]);

#ifdef __cplusplus
}
#endif

#endif /* PERFMONMNGR_H_ */
