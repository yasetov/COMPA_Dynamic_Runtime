//#include <locale.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "xil_cache.h"
//#include "xuartps.h"
//#include "xparameters.h"
//#include "ff.h"
//#include "hdmi/my_cf_hdmi_demo.h"
//#include "hdmi/my_cf_hdmi.h"
#include "xil_printf.h"
#include "config.h"
//#include "mappings.h"
//#include "fifoAllocations.h"
//#include "ctrl_fifos.h"
#include "system.h"
#include "ctrl_fifo_functions.h"
#include "compa_trace.h"
#include "mappings.h"
#include "ctrlComms.h"
#include "timerMngr.h"
#include "perfMonMngr.h"

//--- Global variables allocated here and used in other files ---//
//u8 refFrmBuff[IMG_LENGTH];
u32 currNbProc = 1;
u32 prevNbProc;
ctrl_fifo_hdlr ctrl_fifo_output[NB_PROCESSORS];		/* Control fifos*/
ctrl_fifo_hdlr ctrl_fifo_input[NB_PROCESSORS];
ctrl_fifo_hdlr input_data;							/* Fifo for input data */


u64 execTimes[NB_PROCESSORS][NB_ACTORS+1] = {0}; // The last element stores the global execution time!
u32 writtenBytes[NB_PROCESSORS][NB_ACTORS+1] = {0}; // The last element stores the total number of written bytes!
u32 nbFrames = 0;											// Frames per Seconds metric
u64 nbCycles[NB_PROCESSORS];
//-------//

int main(int argc, char *argv[]){
	u8 i;
	u16 nbFrm = 0;
	u8* decPictureBuffer;

	init_system();

#if ENABLE_FIFOS_COPY == 1
	initMemRegion(FIFOS_COPY_BUFFS_START_ADDR, sizeof(FIFOS_COPY_BUFF)*NB_FIFOS);
#endif

#if ENABLE_START_SIGNAL == 1
#if DDR_DATA_FIFO == 1
	/* Initializing memory region for data fifos */
	initMemRegion(DDR_DATA_FIFO_START, DDR_DATA_FIFO_SIZE);
#endif

	/* Sending start signal to all slave processors */
	if(sendStartSignalToAll() > 0) return -1;
#endif


	/* Broadcast "init_done" message */
	for (i = 0; i < NB_PROCESSORS; i++) {
		sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_INIT_DONE);
	}

	xil_printf("Executing with %d processor(s)\n\r", currNbProc);
//	initMapping();
	initMappingEx(currNbProc);

	xil_printf("0 : Execution with 1 processor(s)\n\r");
	xil_printf("1 : Execution with 2 processor(s)\n\r");
	xil_printf("2 : Execution with 4 processor(s)\n\r");
	xil_printf("3 : Execution with 8 processor(s)\n\r");
	xil_printf("4 : Execution with 16 processor(s)\n\r");
	xil_printf("Enter a value between 0 and 4: ");

//		cmpFifosWithTraces();
		while(1){
#if ENABLE_TRACES == 1
			u8 msg;
			char proc;
			TVerifyDataFifo fifoArray;
			for (proc = 0; proc < currNbProc; proc++) {
				msg = rcvCtrlMsgType(&ctrl_fifo_input[proc]);
				switch (msg) {
					case MSG_VERIFY_DATA_FIFOS:
						while(pop_contents_input_ctrl_fifo_contents_ex(&ctrl_fifo_input[proc], (u8*)&fifoArray, sizeof(TVerifyDataFifo))){
							if(verifyDataFifoVsTraces(&fifoArray) != -1){
								sendCtrlMsgType(&ctrl_fifo_output[proc], MSG_VERIFY_DATA_FIFOS_ERROR);
							}
						}
						sendCtrlMsgType(&ctrl_fifo_output[proc], MSG_VERIFY_DATA_FIFOS_OK);
						break;
					default:
						break;
				}
			}
#endif
//			printFifoIndices(XPAR_CPU_ID);
			if(changeMapping(&prevNbProc, &currNbProc)){
//				printFifoIndices(XPAR_CPU_ID);

				broadcastGetMetricsMsg_blocking(prevNbProc, execTimes, writtenBytes, &nbFrames);

//				printExecTimes(prevNbProc, execTimes);
//				printWrittenBytesValues(prevNbProc, writtenBytes);
				printFPS(nbFrames, execTimes[0][NB_ACTORS]);

				broadcastFlushMsg_blocking(prevNbProc); // Sending flush messages and waiting for flush completed msg from all active processors.

				xil_printf("Executing with %d processor(s)\n\r", currNbProc);
				dynamicMapping(prevNbProc, currNbProc, execTimes);

				xil_printf("\nEnter a value between 0 and 4: ");
			}
		}
//		while(rd_ctrl_fifo_nb_elements(&ctrl_fifo_input[0]) == 0);
//
//		source_readRefFrm(refFrmBuff);	// Read frame from reference file.
//		if(rd_ctrl_fifo_nb_elements(&ctrl_fifo_input[0]) >= IMG_LENGTH){
////				pictureBuffer = &ctrl_fifo_input[0].fifo.contents[ctrl_fifo_input[0].ctrl_fifo_rd_ix % ctrl_fifo_input[0].fifo.size];
//			decPictureBuffer = pop_ctrl_fifo_contents(&ctrl_fifo_input[0]); // Points to the contents of FIFO_0 where the image was stored.
//			DDRVideoWr(decPictureBuffer); // Copy image into the video IP frame buffer.
////				DDRVideoWr(refFrmBuff);
//			nbFrm++; // Counts the number of frames.
//			xil_printf("%d\r\n", nbFrm);
//		}

	return 0;
}
