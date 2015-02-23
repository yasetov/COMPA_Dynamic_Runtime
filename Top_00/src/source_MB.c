/*
 * sourceFileMgr.c
 *
 *  Created on: 4 juil. 2014
 *      Author: yolivave
 */


#include "types.h"
#include "config.h"
#include "ctrl_fifo_functions.h"
#if USE_TIMER == 1
#include "xtmrctr.h"
#endif

//---Local variables -------//
#if USE_TIMER == 1
static XTmrCtr TmrCtrInstance;							// Timer driver instance.
static XTmrCtr* TmrCtrInstancePtr = &TmrCtrInstance;
#endif
static int stop = 0;
//------//

//---- External declarations -----//
extern u32 frameCtr;				// At Display.c, counts the decoded frames.

//--- Global variables ---//
u8* bytesRead;
//u8* IMG_DATA;
u8* dec_img_data;

//--- Control FIFOs ---//
#if ENABLE_START_SIGNAL == 1
#define EXT_	extern
#else
#define EXT_
#endif

extern ctrl_fifo_hdlr input_data_fifo;

u32 nbByteRead = 0;

void readData(){
//	do {
//		rd_ctrl_fifo(&input_data_fifo);
//	} while ((input_data_fifo.ctrl_fifo_num_tkn - input_data_fifo.ctrl_fifo_rd_ix)<=0);

	while (!rd_input_ctrl_fifo_nb_elements(&input_data_fifo));

//	bytesRead = input_data_fifo.fifo.contents;
//	IMG_DATA = input_data_fifo.fifo.contents;
//	input_data_fifo.ctrl_fifo_rd_ix += input_data_fifo.ctrl_fifo_wr_ix;
	bytesRead = input_data_fifo.fifo.contents;
//	IMG_DATA = bytesRead;
}

u32 source_sizeOfFile(){
	return (u32)FILE_SIZE;
}

u8 source_isMaxLoopsReached(){
	return 0;
}

void source_init() {
	int Status;
	u32 clocks;

#if USE_TIMER == 1
	// Initializing timer.
	Status = InitTmrCtr();
	if (Status != XST_SUCCESS) {
#if XIL_PRINTF == 1
		xil_printf("Timer initialization failed.\r\n");
#endif
		return XST_FAILURE;
	}
#endif

	// Reading input data.
	readData();
//	updt_rd_ix(&input_data_fifo); // Update read index.
#if XIL_PRINTF == 1
	xil_printf("%d bytes read of input data\r\n", input_data_fifo.ctrl_fifo_rd_ix % (input_data_fifo.fifo.size + 1));
#endif

#if USE_TIMER == 1
	// Start timer.
	XTmrCtr_Start(TmrCtrInstancePtr, TIMER_CNTR_0);
#endif


}


void postExec(){
#if USE_TIMER == 1
	// Read timer
	clocks = XTmrCtr_GetValue(TmrCtrInstancePtr, TIMER_CNTR_0);
#endif

#if XIL_PRINTF == 1 && USE_TIMER == 1
	xil_printf("%d iterations, %d frames\n", stop, frameCtr);
	xil_printf("%d timer clocks at 100MHz\n", clocks);
#endif

#if PRINT_FIFO_INDEX == 1
	printFifoIndices();
#endif
}


void source_readNBytes(bytesRead, local_NB_TOKEN_TO_SENT){
}

void source_decrementNbLoops(){}

void source_rewind(){}


#if PRINT_FIFO_INDEX == 1
void printFifo(fifo_u8_t* fifo, int index){
	int i;
	if(fifo->readers_nb > 1){
		xil_printf("fifo_%d: %d", index, *fifo->write_ind);
		for (i = 0; i < fifo->readers_nb; i++) {
			xil_printf(" %d", fifo->read_inds[i]);
		}
		xil_printf("\n");
	}
	else{
		xil_printf("fifo_%d: %d %d\n", index, *fifo->write_ind, fifo->read_inds[0]);
	}
}

void printFifoIndices(){
	printFifo(&fifo_0, 0);
	printFifo(&fifo_1, 1);
	printFifo(&fifo_2, 2);
	printFifo(&fifo_3, 3);
	printFifo(&fifo_4, 4);
	printFifo(&fifo_5, 5);
	printFifo(&fifo_6, 6);
	printFifo(&fifo_7, 7);
	printFifo(&fifo_8, 8);
	printFifo(&fifo_9, 9);
	printFifo(&fifo_10, 10);
	printFifo(&fifo_11, 11);
	printFifo(&fifo_12, 12);
	printFifo(&fifo_13, 13);
	printFifo(&fifo_14, 14);
	printFifo(&fifo_15, 15);
	printFifo(&fifo_16, 16);
	printFifo(&fifo_17, 17);
	printFifo(&fifo_18, 18);
	printFifo(&fifo_19, 19);
	printFifo(&fifo_20, 20);
	printFifo(&fifo_21, 21);
	printFifo(&fifo_22, 22);
	printFifo(&fifo_23, 23);
	printFifo(&fifo_24, 24);
	printFifo(&fifo_25, 25);
	printFifo(&fifo_26, 26);
	printFifo(&fifo_27, 27);
	printFifo(&fifo_28, 28);
	printFifo(&fifo_29, 29);
	printFifo(&fifo_30, 30);
	printFifo(&fifo_31, 31);
}
#endif

#if USE_TIMER == 1
int InitTmrCtr()
{
	int Status;


	/*
	 * Initialize the TmrCtr driver
	 */
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TIMER_CNTR_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
#endif
