/*
 * config.h
 *
 *  Created on: 7 nov. 2014
 *      Author: yolivave
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//--- Macros --------//
#define IMG_WIDTH		176
#define IMG_HEIGHT		144
#define IMG_LENGTH_Y	IMG_WIDTH * IMG_HEIGHT
#define IMG_LENGTH_U	(IMG_WIDTH/2) * (IMG_HEIGHT/2) //4:2:0 coding
#define IMG_LENGTH 		(IMG_LENGTH_Y + (2*IMG_LENGTH_U))


#define NB_ACTORS			15
#define USE_ACTORS_LIB		0	// Whether use the library which contains all the actors' code. I.e all actors executed by one processor.
#define PRINT_FIFO_INDEX 	0	// Whether to print the indices of Fifos at break.
#define NB_FIFOS			32	// Total of Fifos in the application.
#define TIMER_CNTR_0		0	// Timer index used in the timer IP.
#define USE_TIMER			0
#define XIL_PRINTF			0
#define FILE_SIZE 250342

#define ENABLE_START_SIGNAL			0

#define INPUT_FILE_SIZE			250344 // 0x3D1E8 Size of the encoded input file.
#define INPUT_FILE_START_ADDR	0x40000a00
#define INPUT_FILE_RD_IX_ADDR	INPUT_FILE_START_ADDR + INPUT_FILE_SIZE
#define INPUT_FILE_WR_IX_ADDR	INPUT_FILE_RD_IX_ADDR + 4

#define CTRL_FIFO_IN_0_SIZE 		1024
#define CTRL_FIFO_IN_0_START_ADDR	0x40000000
#define CTRL_FIFO_IN_0_RD_IX_ADDR	CTRL_FIFO_IN_0_START_ADDR + CTRL_FIFO_IN_0_SIZE
#define CTRL_FIFO_IN_0_WR_IX_ADDR	CTRL_FIFO_IN_0_RD_IX_ADDR + 4


#define CTRL_FIFO_OUT_0_SIZE		1024
#define CTRL_FIFO_OUT_0_START_ADDR	0x40000408
#define CTRL_FIFO_OUT_0_RD_IX_ADDR	CTRL_FIFO_OUT_0_START_ADDR + CTRL_FIFO_OUT_0_SIZE
#define CTRL_FIFO_OUT_0_WR_IX_ADDR	CTRL_FIFO_OUT_0_RD_IX_ADDR + 4

#define END_FLAG_SIZE				1 // Bytes written to indicate the end of decoding.
#define GET_NB_FRAMES

#endif /* CONFIG_H_ */
