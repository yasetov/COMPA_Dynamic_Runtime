/*
 * perfMonMngr.h
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#ifndef PERFMONMNGR_H_
#define PERFMONMNGR_H_

#include "xaxipmon.h"

int initPerfMons();

void readPerfMonitor(XAxiPmon *AxiPmonInstPtr,
					u32 Metrics[],
					u32 *ClkCntHigh,
					u32 *ClkCntLow);

void printMetricValues(XAxiPmon *AxiPmonInstPtr, u8 nbSlots, u8 Metric);

#endif /* PERFMONMNGR_H_ */
