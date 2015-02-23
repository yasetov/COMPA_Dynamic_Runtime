/*
 * ctrlComms.c
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */
#include "stdlib.h"
#include "xil_printf.h"
#include "ctrl_fifo_functions.h"
#include "ctrlComms.h"


extern ctrl_fifo_hdlr ctrl_fifo_output[NB_PROCESSORS];		/* Control fifos*/
extern ctrl_fifo_hdlr ctrl_fifo_input[NB_PROCESSORS];

void broadcastFlushMsg_blocking(int nbProcessors){
	int i;
	u8 msg;
	for (i = 0; i < nbProcessors; i++) {
		sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_FLUSH_DCACHE);
	}

	for (i = 0; i < nbProcessors; i++) {
		msg = rcvCtrlMsgType_blocking(&ctrl_fifo_input[i]);
		if(msg != MSG_FLUSH_DCACHE_DONE){
			xil_printf("Received message %d while expecting %d\n\r", msg, MSG_FLUSH_DCACHE_DONE);
			exit(-1);
		}
	}
}

void broadcastGetMetricsMsg_blocking(int nbProcessors, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1], u32 writtenBytes[NB_PROCESSORS][NB_ACTORS + 1]){
	int i;
	u8 msg;

	for (i = 0; i < nbProcessors; i++) {
		sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_GET_METRICS);
	}

	for (i = 0; i < nbProcessors; i++) {
		msg = rcvCtrlMsgType_blocking(&ctrl_fifo_input[i]);
		if(msg != MSG_GET_METRICS_OK){
			xil_printf("Received message %d while expecting %d\n\r", msg, MSG_GET_METRICS_OK);
			exit(-1);
		}
		pop_contents_input_ctrl_fifo_extended_blocking(&ctrl_fifo_input[i], (u8*)execTimes[i], sizeof(u64)*(NB_ACTORS + 1));
		pop_contents_input_ctrl_fifo_extended_blocking(&ctrl_fifo_input[i], (u8*)writtenBytes[i], sizeof(u32)*(NB_ACTORS + 1));
	}
}
