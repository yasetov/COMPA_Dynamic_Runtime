/*
 * init_platform.c
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */

#include "stdlib.h"
#include "string.h"
#include "ff.h"

void initMemRegion(int startAddr, int size){
	Xil_DCacheDisable();
	memset((int*)startAddr, 0, size);
	Xil_DCacheEnable();
}


void init_system(){
	/* Initializing timers */
	initTimers();

	/* Initializing control FIFOs */
	initCtrlFIFOs();

	/* Initializing performance monitors */
	initPerfMons();

	/* Initializing .bss section to 0*/
	/*TODO: ..code this properly..*/
//	initMemRegion(0x200496e0, 0x0001a0cc);

	/* Initializes the SD card controller. */
	FRESULT rc = initSD();
	if(rc != FR_OK){
		xil_printf("Error %u mounting file system\n\r", rc);
		exit(-1);
	}

	/* Initializes the HDMI controller system */
	initHDMI();

	/* Writing input data into microblaze 0's local memory*/
	writeData();

	init_mappings();
}
