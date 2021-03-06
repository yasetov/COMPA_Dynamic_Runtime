/*
 * mappings.c
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */

#include <stdlib.h>
#include "xparameters_ps.h"
#include "xuartps_hw.h"
#include "actors.h"
#include "mappings.h"
#include "ctrlComms.h"
#include "ctrl_fifo_functions.h"

extern ctrl_fifo_hdlr ctrl_fifo_output[NB_PROCESSORS];		/* Control fifos*/
extern ctrl_fifo_hdlr ctrl_fifo_input[NB_PROCESSORS];

static bool mapping [NB_PROCESSORS][NB_ACTORS] = {false};

static void setMapping00() { // Mapping index for 1 processor

	// Processor index 0
	mapping[0][decoder_merger] = true;
	mapping[0][decoder_parser_parseheaders] = true;
	mapping[0][decoder_parser_mvseq] = true;
	mapping[0][decoder_parser_blkexp] = true;
	mapping[0][decoder_parser_mvrecon] = true;
	mapping[0][decoder_texture_DCsplit] = true;
	mapping[0][decoder_texture_IS] = true;
	mapping[0][decoder_texture_IAP] = true;
	mapping[0][decoder_texture_IQ] = true;
	mapping[0][decoder_texture_idct2d] = true;
	mapping[0][decoder_texture_DCReconstruction_addressing] = true;
	mapping[0][decoder_texture_DCReconstruction_invpred] = true;
	mapping[0][decoder_motion_interpolation] = true;
	mapping[0][decoder_motion_add] = true;
	mapping[0][decoder_motion_framebuf] = true;
}

static void setMapping01() { // Mapping for 2 processors

	// Processor index 0
	mapping[0][decoder_merger] = false;
	mapping[0][decoder_parser_parseheaders] = false;
	mapping[0][decoder_parser_mvseq] = false;
	mapping[0][decoder_parser_blkexp] = false;
	mapping[0][decoder_parser_mvrecon] = false;
	mapping[0][decoder_texture_DCsplit] = false;
	mapping[0][decoder_texture_IS] = false;
	mapping[0][decoder_texture_IAP] = false;
	mapping[0][decoder_texture_IQ] = false;
	mapping[0][decoder_texture_idct2d] = false;
	mapping[0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[0][decoder_motion_interpolation] = false;
	mapping[0][decoder_motion_add] = false;
	mapping[0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[1][decoder_merger] = true;
	mapping[1][decoder_parser_parseheaders] = true;
	mapping[1][decoder_parser_mvseq] = true;
	mapping[1][decoder_parser_blkexp] = true;
	mapping[1][decoder_parser_mvrecon] = true;
	mapping[1][decoder_texture_DCsplit] = true;
	mapping[1][decoder_texture_IS] = true;
	mapping[1][decoder_texture_IAP] = true;
	mapping[1][decoder_texture_IQ] = true;
	mapping[1][decoder_texture_idct2d] = true;
	mapping[1][decoder_texture_DCReconstruction_addressing] = true;
	mapping[1][decoder_texture_DCReconstruction_invpred] = true;
	mapping[1][decoder_motion_interpolation] = true;
	mapping[1][decoder_motion_add] = true;
	mapping[1][decoder_motion_framebuf] = true;
}


void initMapping(){
	int j;
	int tempSchedCaller[NB_ACTORS];
	for (j = 0; j < NB_ACTORS; j++) {
		tempSchedCaller[j] = actors[j];
	}
	sendCtrlMsgType_blocking(&ctrl_fifo_output[0], MSG_ACTORS_MAP);
	push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output[0], (u8*)tempSchedCaller, sizeof(tempSchedCaller));
}

void initMappingEx(u32 currNbProc){
	int i, j;
	int tempSchedCaller[NB_ACTORS];
	int cntr = 0;

	memset(mapping, false, sizeof(mapping));
	switch (currNbProc) {
		case 1:
			setMapping00();
			break;
		case 2:
			setMapping01();
			break;
	}

	for (i = 0; i < currNbProc; i++) {
		cntr = 0;
		for (j = 0; j < NB_ACTORS; j++) {
			if(mapping[i][j] == true){
//				tempSchedCaller[cntr] = schedCallerAddr[j];
				tempSchedCaller[cntr] = actors[j];
				cntr++;
			}
		}
		if (cntr > 0){
			sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_ACTORS_MAP);
			push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output[i], (u8*)tempSchedCaller, cntr * sizeof(int));
		}
	}
}

int compare (const void * a, const void * b)
{
  if ( *(u64*)a <  *(u64*)b ) return -1;
  if ( *(u64*)a == *(u64*)b ) return 0;
  if ( *(u64*)a >  *(u64*)b ) return 1;
}

void loadBalancing(u32 prevNbProc, u32 currNbProc, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1]){
	u32 i, j, proc;
	u64 flatExecTime[NB_ACTORS] = {0};

	for (i = 0; i < prevNbProc - 1; i++) {
		for (j = 0; j < NB_ACTORS; ++j) {
			flatExecTime[j] += execTimes[i][j];
			if(flatExecTime[j] > 0) break; // Stop when the execution time of actor j is found.
		}
	}
	qsort(flatExecTime, NB_ACTORS, sizeof(u64), compare);

	for (i = 0; i < NB_ACTORS; i++) {
		proc = i % (currNbProc - 1); // currNbProc - 1 cause processor 0 doesn't count
		mapping[proc + 1][i] = true; // idem, proc + 1 to avoid processor 0
	}
}

void dynamicMapping(u32 prevNbProc, u32 currNbProc, u64 execTimes[NB_PROCESSORS][NB_ACTORS + 1]){
	int i, j;
	int tempSchedCaller[NB_ACTORS];
	int cntr = 0;

	memset(mapping, false, sizeof(mapping));
	switch (currNbProc) {
		case 1:
			setMapping00();
			break;
		case 2:
			setMapping01();
			break;
		default:
			loadBalancing(prevNbProc, currNbProc, execTimes);
			break;
	}

	for (i = 0; i < currNbProc; i++) {
		cntr = 0;
		for (j = 0; j < NB_ACTORS; j++) {
			if(mapping[i][j] == true){
//				tempSchedCaller[cntr] = schedCallerAddr[j];
				tempSchedCaller[cntr] = actors[j];
				cntr++;
			}
		}
		if (cntr > 0){
			sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_ACTORS_MAP);
			push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output[i], (u8*)tempSchedCaller, cntr * sizeof(int));
		}
	}
}

u8 changeMapping(u32* prevNbProc, u32* currNbProc){
	char receivedChar = 0;
	while(!XUartPs_IsReceiveData(XPS_UART1_BASEADDR));
	receivedChar = inbyte();
//		if(((receivedChar == 0x31) || (receivedChar == 0x32) || (receivedChar == 0x34) || (receivedChar == 0x38) || (receivedChar == 0x40)) && ((receivedChar - 0x30) != currNbProc)){
	*prevNbProc = *currNbProc;
	switch (receivedChar - 0x30) {
		case 0:
			*currNbProc = 1;
			break;
		case 1:
			*currNbProc = 2;
			break;
		case 2:
			*currNbProc = 4;
			break;
		case 3:
			*currNbProc = 8;
			break;
		case 4:
			*currNbProc = 16;
			break;
	}
	if(*currNbProc != *prevNbProc)
//			currNbProc = receivedChar - 0x30;
//			currNbProc = 2 << (receivedChar - 0x30);
			return 1;
	else
		return 0;
}
