/*
 * config.h
 *
 *  Created on: 5 nov. 2014
 *      Author: yolivave
 */

#ifndef CONFIG_H_
#define CONFIG_H_


//--- General macros --------//
#define MAX_NUM_MONITOR_SLOTS 	8	//
#define MAX_NB_INCREMENTERS		8
#define USE_PERF_MON			1
//#define DDR_DATA_FIFO			1	// Whether the DDR is used to allocate data fifos.
//#if DDR_DATA_FIFO == 1
//	#define DDR_DATA_FIFO_START		0x20000000
//	#define DDR_DATA_FIFO_SIZE		0X7A00
//#endif
#define ENABLE_TRACES			0
#define ENABLE_FIFOS_COPY		0
#define END_FLAG_SIZE			1 // Bytes read to indicate the end of decoding.

#define NB_PROCESSORS			16
#define NB_ACTORS				15
//-----//

#define ENABLE_START_SIGNAL			0

//-------//


#endif /* CONFIG_H_ */
