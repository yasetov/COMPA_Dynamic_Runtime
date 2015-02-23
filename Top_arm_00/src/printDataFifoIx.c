/*
 * printDataFifoIx.c
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */
#include "fifoAllocations.h"



void printFifo(u8 cpuId, fifo_u8_t* fifo, int index){
	int i;
	if(fifo->readers_nb > 1){
		xil_printf("CPU %d...fifo_%d: %d", cpuId, index, *fifo->write_ind);
		for (i = 0; i < fifo->readers_nb; i++) {
			xil_printf(" %d", fifo->read_inds[i]);
		}
		xil_printf("\n");
	}
	else{
		xil_printf("CPU %d...fifo_%d: %d %d\n", cpuId, index, *fifo->write_ind, fifo->read_inds[0]);
	}
}

void printFifoIndices(u8 cpuId){
	printFifo(cpuId, (fifo_u8_t*)&fifo_0, 0);
	printFifo(cpuId, (fifo_u8_t*)&fifo_1, 1);
	printFifo(cpuId, (fifo_u8_t*)&fifo_2, 2);
	printFifo(cpuId, (fifo_u8_t*)&fifo_3, 3);
	printFifo(cpuId, (fifo_u8_t*)&fifo_4, 4);
	printFifo(cpuId, (fifo_u8_t*)&fifo_5, 5);
	printFifo(cpuId, (fifo_u8_t*)&fifo_6, 6);
	printFifo(cpuId, (fifo_u8_t*)&fifo_7, 7);
	printFifo(cpuId, (fifo_u8_t*)&fifo_8, 8);
	printFifo(cpuId, (fifo_u8_t*)&fifo_9, 9);
	printFifo(cpuId, (fifo_u8_t*)&fifo_10, 10);
	printFifo(cpuId, (fifo_u8_t*)&fifo_11, 11);
	printFifo(cpuId, (fifo_u8_t*)&fifo_12, 12);
	printFifo(cpuId, (fifo_u8_t*)&fifo_13, 13);
	printFifo(cpuId, (fifo_u8_t*)&fifo_14, 14);
	printFifo(cpuId, (fifo_u8_t*)&fifo_15, 15);
	printFifo(cpuId, (fifo_u8_t*)&fifo_16, 16);
	printFifo(cpuId, (fifo_u8_t*)&fifo_17, 17);
	printFifo(cpuId, (fifo_u8_t*)&fifo_18, 18);
	printFifo(cpuId, (fifo_u8_t*)&fifo_19, 19);
	printFifo(cpuId, (fifo_u8_t*)&fifo_20, 20);
	printFifo(cpuId, (fifo_u8_t*)&fifo_21, 21);
	printFifo(cpuId, (fifo_u8_t*)&fifo_22, 22);
	printFifo(cpuId, (fifo_u8_t*)&fifo_23, 23);
	printFifo(cpuId, (fifo_u8_t*)&fifo_24, 24);
	printFifo(cpuId, (fifo_u8_t*)&fifo_25, 25);
	printFifo(cpuId, (fifo_u8_t*)&fifo_26, 26);
	printFifo(cpuId, (fifo_u8_t*)&fifo_27, 27);
	printFifo(cpuId, (fifo_u8_t*)&fifo_28, 28);
	printFifo(cpuId, (fifo_u8_t*)&fifo_29, 29);
	printFifo(cpuId, (fifo_u8_t*)&fifo_30, 30);
	printFifo(cpuId, (fifo_u8_t*)&fifo_31, 31);
	xil_printf("----------------------------------------\n");
}

