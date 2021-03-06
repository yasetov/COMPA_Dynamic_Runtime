/*
 * sourceFileMgr.c
 *
 *  Created on: 4 juil. 2014
 *      Author: yolivave
 */

#include <stdlib.h>
#include <string.h>
#include "ff.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "hdmi/my_cf_hdmi_demo.h"
#include "types.h"
#include "config.h"
#if ENABLE_TRACES == 1
#include "tracesDefs.h"
#endif
#include "ctrl_fifo_functions.h"
#include "compa_trace.h"
#include "fileMngr.h"


extern void xil_printf( const char *ctrl1, ...);
extern ctrl_fifo_hdlr input_data;							/* Fifo for input data */

static FATFS fatfs;
static FIL fil;
static FIL filRef;
static unsigned int nbByteRead = 0;


FRESULT initSD(){
	FRESULT rc = FR_OK;

	/* Register volume work area, initialize device */
	rc = f_mount(0, &fatfs);
	return rc;
}


//void readImgFromFile(){
//	FRESULT rc = FR_OK;
//	UINT bytesRead = 0;
//	static UINT imgNb = 0;
//
//	Xil_DCacheDisable();
////	Xil_ICacheDisable();
//
//	// Opening video file.
////	rc = f_open(&fil, "lena.rgb", FA_READ);
//	rc = f_open(&fil, "suzie.yuv", FA_READ);
//	if (rc != FR_OK) {
//		xil_printf("f_open failed: %d\n", rc);
//	}
//
//	//	memset(IMG_DATA, 0, IMG_LENGTH);
//	rc = f_lseek(&fil, IMG_LENGTH * imgNb);
//	if(imgNb < NB_FRMS)
//		imgNb++;
//	else
//		imgNb = 0;
//
//	rc = f_read(&fil, IMG_DATA, IMG_LENGTH, &bytesRead);
//	if(rc != FR_OK){
//		xil_printf("f_read failed: %d\n", rc);
//	}
//
//	rc = f_close(&fil);
//	if (rc != FR_OK) {
//		xil_printf("SD_INIT_FAIL: %d\n", rc);
//	}
//
//	Xil_DCacheEnable();
//}


unsigned int source_sizeOfFile(){
	return fil.fsize;
}

int source_readNBytes(unsigned char *outTable, unsigned int nbBytesToRead){
	FRESULT rc = FR_OK;
	UINT bytesRead = 0;

	Xil_DCacheDisable();

	rc = f_read(&fil, outTable, nbBytesToRead, &bytesRead);
	if(rc != FR_OK){
	}
	else{
		if (bytesRead < nbBytesToRead) {
			if (f_eof(&fil)) {
//				xil_printf("warning\n");
				f_lseek(&fil, 0);
				rc = f_read(&fil, outTable, 1, &bytesRead);
				if(rc != FR_OK){
//					xil_printf("f_read failed: %d\n", rc);
					return bytesRead;
				}
			}
			else {
//				xil_printf("Problem when reading input file.\n");
				return bytesRead;
			}
		}
	}
	Xil_DCacheEnable();

	nbByteRead = nbBytesToRead * 8;
	return bytesRead;
}

int source_readRefFrm(u8* refFrmBuff){
	FRESULT rc = FR_OK;
	UINT bytesRead = 0;

	Xil_DCacheDisable();

	rc = f_read(&filRef, refFrmBuff, IMG_LENGTH, &bytesRead);
	if(rc != FR_OK) {
		xil_printf("Error %d reading reference frame\n\r", rc);
	}

	if (bytesRead < IMG_LENGTH) {
		if (f_eof(&filRef)) {
			f_lseek(&filRef, 0);
			rc = f_read(&filRef, refFrmBuff, IMG_LENGTH, &bytesRead);
			if(rc != FR_OK){
				xil_printf("f_read failed: %d\n\r", rc);
			}
		}
		else {
//			fprintf(stderr,"Problem when reading input file.\n");
		}
	}

	Xil_DCacheEnable();
	return bytesRead;
}

unsigned int source_readByte(){
	FRESULT rc = FR_OK;
	unsigned char buf;
	UINT bytesRead = 0;

	Xil_DCacheDisable();

	rc = f_read(&fil, &buf, 1, &bytesRead);
	if(rc != FR_OK){
		xil_printf("f_read failed: %d\n\r", rc);
	}

	if (bytesRead < 1) {
		if (f_eof(&fil)) {
			xil_printf("warning\n");
			f_lseek(&fil, 0);
			rc = f_read(&fil, &buf, 1, &bytesRead);
			if(rc != FR_OK){
				xil_printf("f_read failed: %d\n\r", rc);
			}
		}
		else {
//			fprintf(stderr,"Problem when reading input file.\n");
		}
	}

	Xil_DCacheEnable();

	nbByteRead += 8;
	return buf;
}


void source_rewind() {
	FRESULT rc = FR_OK;
	rc = f_lseek(&fil, 0);
	if (rc != FR_OK) {
		xil_printf("f_lseek failed: %d\n", rc);
	}
}

void source_init() {
	FRESULT rc = FR_OK;
	char fileName[30];

	// Opening video file.
	Xil_DCacheDisable();
//		rc = f_open(&fil, "tul.yuv", FA_READ);

	strcpy(fileName, "fore.bit");
	rc = f_open(&fil, fileName, FA_READ);
	if(rc != FR_OK){
		xil_printf("Error %d opening file %s\n\r", rc, fileName);
		exit(-1);
	}

	strcpy(fileName, "foreD.yuv");
	rc = f_open(&filRef, fileName, FA_READ);
	if(rc != FR_OK){
		xil_printf("Error %d opening file %s\n\r", rc, fileName);
		exit(-1);
	}
	Xil_DCacheEnable();
}

void source_close(){
	FRESULT rc = FR_OK;
	rc = f_close(&fil);
	if(rc != FR_OK){
		xil_printf("Error %d closing file \n\r", rc);
		exit(-1);
	}
	rc = f_close(&filRef);
	if(rc != FR_OK){
		xil_printf("Error %d closing file \n\r", rc);
		exit(-1);
	}
}



int copyVData(int bytesToCopy){
	return source_readNBytes(input_data.fifo.contents, bytesToCopy);
}

void writeData(){
	u32 bytesRead;
	u32 bytesToCopy;
	u32 fileSize = 0;

	 /* Open the encoded video file */
	source_init();

	fileSize = source_sizeOfFile();
	bytesToCopy = fileSize;

//	do {
//		rd_ctrl_fifo(&ctrl_fifo_output[0]);
//	} while (1 > ctrl_fifo_output[0].fifo.size - ctrl_fifo_output[0].ctrl_fifo_wr_ix + ctrl_fifo_output[0].ctrl_fifo_rd_ix);

	while(bytesToCopy > rd_output_ctrl_fifo_nb_free(&input_data));

	bytesRead = copyVData(bytesToCopy);
	if(bytesRead == bytesToCopy){
		xil_printf("%d bytes of %d were copied.\n\r", bytesRead, fileSize);
		input_data.fifo.write_ind[0] += bytesRead;
	}else{
		xil_printf("Incomplete copy : only %d bytes of %d were copied.\n\r", bytesRead, fileSize);
		exit(-1);
	}

	source_close();
}



int verifyDataFifoVsTraces(TVerifyDataFifo* fifo){
	FIL file;
	FRESULT rc = FR_OK;
	char filePath[80];
	strcpy(filePath, "/traces/");
	strcat(filePath, fifo->traceFileName);
	strcat(filePath, ".txt");

	Xil_DCacheDisable();
	// Opening file.
	rc = f_open(&file, filePath, FA_READ);
	if(rc != FR_OK){
		xil_printf("Error %d at f_open\n\r", rc);
		exit(-1);
	}

//	rc = f_lseek(&file, 600);

	// Find starting position in file.
	int lineNo = 0;
	char traceLn[10];
	while(lineNo < fifo->startIx){
		f_gets(traceLn, sizeof(traceLn), &file);				// Read a line from the file.
		lineNo++;
	}

	// Compare file's vs fifo's data from 'startIx' to 'endIx'.
	int i, fileData, fifoData;
	for (i = fifo->startIx; i < fifo->endIx; i++) {
		memset(traceLn, 0, sizeof(traceLn));				// Clear string.
		TCHAR result = *f_gets(traceLn, sizeof(traceLn), &file);			// Read a line from trace file.
		fileData = atoi(traceLn);

		// Fifo contents can be of any type (e.g. u8 or i16) so i need to..Note that indices for reading the fifo must be modulated.
		switch (fifo->tokenSz) {
			case 1:
				fifoData = ((u8*)fifo->tokens)[i % fifo->fifoSz];
				break;
			case 2:
				fifoData = ((u16*)fifo->tokens)[i % fifo->fifoSz];
				break;
			case 4:
				fifoData = ((u32*)fifo->tokens)[i % fifo->fifoSz];
				break;
			default:
				break;
		}
		if(memcmp(&fifoData, &fileData, sizeof(fifo->tokenSz))){
			// Values are not equal
			Xil_DCacheEnable();
			return i;
		}
	}
	Xil_DCacheEnable();
	return -1;
}


#if ENABLE_TRACES == 1
/*
 * Opens the trace files corresponding to the output ports of the actor  "actorNo".
 * The values in the files are placed into buffers MAX_TRACE_BUFF_SIZE-int-elements long.
 **/
void openTracesFiles(){
	FRESULT rc = FR_OK;
	FIL fil;
	int actorNo, port, lineNo;
	char traceLn[10];

//	tODO : rc = initSD();
	if(rc == FR_OK){
		Xil_DCacheDisable();
		for (actorNo = 0; actorNo < NB_ACTORS - 1; actorNo++) {
			for (port = 0; port < MAX_NB_OUT_PORTS; port++) {
				if(tracesNames[actorNo][port]){
					rc = f_open(&fil, tracesNames[actorNo][port], FA_READ);
					lineNo = 0;
					while(lineNo < MAX_TRACE_BUFF_SIZE || !f_eof(&fil)){
						memset(traceLn, 0, sizeof(traceLn));				// Clear string.
						f_gets(traceLn, sizeof(traceLn), &fil);				// Read a line from trace file.
						tracesBuffs[actorNo].values[port][lineNo] = atoi(traceLn);	// Store integer value.
						lineNo++;
					}
					rc = f_close(&fil);
				}else
					break;

			}
			// Clearing the indices of the traces' buffers.
			memset(tracesBuffs[actorNo].indices, 0, sizeof(tracesBuffs[actorNo].indices));
		}
		Xil_DCacheEnable();
	}

}
//FIL fil[NB_ACTORS][MAX_NB_OUT_PORTS];
//void openTracesFiles(){
//	FRESULT rc = FR_OK;
//	FIL fil;
//	int actorNo, port, lineNo;
//	char traceLn[10];
//
////	tODO : rc = initSD();
//	if(rc == FR_OK){
//		Xil_DCacheDisable();
//		for (actorNo = 0; actorNo < NB_ACTORS - 1; actorNo++) {
//			for (port = 0; port < MAX_NB_OUT_PORTS; port++) {
//				if(tracesNames[actorNo][port]){
//					rc = f_open(&fil[actorNo][port], tracesNames[actorNo][port], FA_READ);
//				}else
//					break;
//
//			}
//		}
//		Xil_DCacheEnable();
//	}
//}

#endif
