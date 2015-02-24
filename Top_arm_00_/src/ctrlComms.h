/*
 * ctrlComms.h
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */

#ifndef CTRLCOMMS_H_
#define CTRLCOMMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
//
//extern ctrl_fifo_hdlr ctrl_fifo_output[NB_PROCESSORS];		/* Control fifos*/
//extern ctrl_fifo_hdlr ctrl_fifo_input[NB_PROCESSORS];

void broadcastFlushMsg_blocking(int nbProcessors);
void broadcastGetMetricsMsg_blocking(int nbProcessors, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1], u32 writtenBytes[NB_PROCESSORS][NB_ACTORS + 1], u32* nbFrames);

#ifdef __cplusplus
}
#endif

#endif /* CTRLCOMMS_H_ */
