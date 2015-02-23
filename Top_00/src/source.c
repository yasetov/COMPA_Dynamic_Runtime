// Source file is "L/RVC/src/org/sc29/wg11/common/Source.cal"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataflow.h"
#include "fifoAllocations.h"
#include "timerMngr.h"

extern u32 frameCtr;
extern u8 fps;
u64 startTime, stopTime, diffTime;
TIME_DATA execTime;

////////////////////////////////////////////////////////////////////////////////
// Output FIFOs
//extern fifo_u8_t *source_O;
//#define source_O_ix		0

////////////////////////////////////////////////////////////////////////////////
// Output Fifo control variables
static unsigned int index_O;
static unsigned int numFree_O;
#define NUM_READERS_O 1
#define SIZE_O 512
#define tokens_O source_O->contents



////////////////////////////////////////////////////////////////////////////////
// State variables of the actor
//#define Source_NB_TOKEN_TO_SENT 4096
#define Source_NB_TOKEN_TO_SENT 250342
extern u8* bytesRead;
static u32 iterationLoop = 0;
static u32 nbByteLeft;
static u32 nbByteToSend = 0;
static u32 nbByteSent = 0;
static u32 nbIteration;

////////////////////////////////////////////////////////////////////////////////
// Initial FSM state of the actor
enum states {
	my_state_ReadFile,
	my_state_ReadFileDone,
	my_state_ReadInit,
	my_state_SendData,
	my_state_SendLastData
};

static char *stateNames[] = {
	"ReadFile",
	"ReadFileDone",
	"ReadInit",
	"SendData",
	"SendLastData"
};

static enum states _FSM_state;
static u8 init = 0;

////////////////////////////////////////////////////////////////////////////////
// Token functions

static void write_O() {
	index_O = (*source_O->write_ind);
	numFree_O = index_O + fifo_u8_get_room(source_O, NUM_READERS_O);
}

static void write_end_O() {
	(*source_O->write_ind) = index_O;
}

////////////////////////////////////////////////////////////////////////////////
// Functions/procedures
extern i32 source_sizeOfFile();
extern i32 source_isMaxLoopsReached();
extern void source_readNBytes(u8 outTable[4096], u32 nbTokenToRead);
extern void source_decrementNbLoops();
extern void source_rewind();
extern void source_init();


////////////////////////////////////////////////////////////////////////////////
// Actions
static i32 isSchedulable_getFileSize() {
	i32 result;
	i32 tmp_source_sizeOfFile;
	i32 tmp_source_isMaxLoopsReached;

	tmp_source_sizeOfFile = source_sizeOfFile();
	tmp_source_isMaxLoopsReached = source_isMaxLoopsReached();
	result = tmp_source_sizeOfFile > 0 && !tmp_source_isMaxLoopsReached;
	return result;
}

static void getFileSize() {

	// Compute aligned port indexes

	u32 nbBytes;
	u32 local_NB_TOKEN_TO_SENT;

	nbBytes = source_sizeOfFile();
	local_NB_TOKEN_TO_SENT = Source_NB_TOKEN_TO_SENT;
	nbIteration = nbBytes / local_NB_TOKEN_TO_SENT;
	local_NB_TOKEN_TO_SENT = Source_NB_TOKEN_TO_SENT;
	nbByteLeft = nbBytes % local_NB_TOKEN_TO_SENT;

	// Update ports indexes

}
static i32 isSchedulable_readNBytes() {
	i32 result;
	u32 local_iterationLoop;
	u32 local_nbIteration;

	local_iterationLoop = iterationLoop;
	local_nbIteration = nbIteration;
	result = local_iterationLoop < local_nbIteration;
	return result;
}

static void readNBytes() {

	// Compute aligned port indexes

	u32 local_NB_TOKEN_TO_SENT;
	u32 local_iterationLoop;

	local_NB_TOKEN_TO_SENT = Source_NB_TOKEN_TO_SENT;
	source_readNBytes(bytesRead, local_NB_TOKEN_TO_SENT);
	local_NB_TOKEN_TO_SENT = Source_NB_TOKEN_TO_SENT;
	nbByteToSend = local_NB_TOKEN_TO_SENT;
	nbByteSent = 0;
	local_iterationLoop = iterationLoop;
	iterationLoop = local_iterationLoop + 1;

	// Update ports indexes

}
static i32 isSchedulable_readEndOfFile() {
	i32 result;
	u32 local_nbByteLeft;

	local_nbByteLeft = nbByteLeft;
	result = local_nbByteLeft > 0;
	return result;
}

static void readEndOfFile() {

	// Compute aligned port indexes

	u32 local_nbByteLeft;

	local_nbByteLeft = nbByteLeft;
	source_readNBytes(bytesRead, local_nbByteLeft);
	local_nbByteLeft = nbByteLeft;
	nbByteToSend = local_nbByteLeft;
	nbByteSent = 0;
	source_decrementNbLoops();

	// Update ports indexes

}
static i32 isSchedulable_readFileDone() {
	i32 result;

	result = 1;
	return result;
}

static void readFileDone() {

	// Compute aligned port indexes

	// Computing FPS.
	stopTime = getTimerValue64();
	diffTime = stopTime - startTime;
	fillTimeData(diffTime, &execTime);
	fps = frameCtr/execTime.seconds;


	iterationLoop = 0;
	source_rewind();
	frameCtr = 0;

	// Update ports indexes

}
static i32 isSchedulable_sendData_launch() {
	i32 result;
	u32 local_nbByteToSend;
	u32 local_nbByteSent;

	local_nbByteToSend = nbByteToSend;
	local_nbByteSent = nbByteSent;
	result = local_nbByteToSend > local_nbByteSent;
	return result;
}

static void sendData_launch() {

	// Compute aligned port indexes
	i32 index_aligned_O = index_O % SIZE_O;

	u8 Out;
	u32 local_nbByteSent;

	local_nbByteSent = nbByteSent;
	Out = bytesRead[local_nbByteSent];
	local_nbByteSent = nbByteSent;
	nbByteSent = local_nbByteSent + 1;
	tokens_O[(index_O + (0)) % SIZE_O] = Out;

	// Update ports indexes
	index_O += 1;

	write_end_O();
}
static i32 isSchedulable_sendData_done() {
	i32 result;
	u32 local_nbByteToSend;
	u32 local_nbByteSent;

	local_nbByteToSend = nbByteToSend;
	local_nbByteSent = nbByteSent;
	result = local_nbByteToSend == local_nbByteSent;
	return result;
}

static void sendData_done() {

	// Compute aligned port indexes



	// Update ports indexes

}


////////////////////////////////////////////////////////////////////////////////
// Initializes
static i32 isSchedulable_untagged_0() {
	i32 result;

	result = 1;
	return result;
}

static void untagged_0() {

	// Compute aligned port indexes


	source_init();

	// Update ports indexes

}

void source_initialize() {
	int i = 0;
	/* Set initial state to current FSM state */
	_FSM_state = my_state_ReadInit;
	// Action loop
	if (isSchedulable_untagged_0()) {
		int stop = 0;
		if (stop != 0) {
			goto finished;
		}
		untagged_0();
		i++;
	} else {
		goto finished;
	}
	
finished:
	// no read_end/write_end here!
	return;
}

////////////////////////////////////////////////////////////////////////////////
// Action scheduler

int source_scheduler() {
	int i = 0;
	if(!init){
		source_initialize();
		init = 1;
	}

	write_O();

	u32 start_index_O = index_O;

	// jump to FSM state 
	switch (_FSM_state) {
	case my_state_ReadFile:
		goto l_ReadFile;
	case my_state_ReadFileDone:
		goto l_ReadFileDone;
	case my_state_ReadInit:
		goto l_ReadInit;
	case my_state_SendData:
		goto l_SendData;
	case my_state_SendLastData:
		goto l_SendLastData;
	default:
		exit(1);
	}

	// FSM transitions
l_ReadFile:
	if (isSchedulable_readNBytes()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_ReadFile;
			goto finished;
		}
		readNBytes();
		i++;
		goto l_SendData;
	} else if (isSchedulable_readEndOfFile()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_ReadFile;
			goto finished;
		}
		readEndOfFile();
		i++;
		goto l_SendLastData;
	} else if (isSchedulable_readFileDone()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_ReadFile;
			goto finished;
		}
		readFileDone();
		i++;
		goto l_ReadFile;
	} else {
		_FSM_state = my_state_ReadFile;
		goto finished;
	}
l_ReadFileDone:
	if (isSchedulable_readFileDone()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_ReadFileDone;
			goto finished;
		}
		readFileDone();
		i++;
		goto l_ReadInit;
	} else {
		_FSM_state = my_state_ReadFileDone;
		goto finished;
	}
l_ReadInit:
	if (isSchedulable_getFileSize()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_ReadInit;
			goto finished;
		}
		getFileSize();
		startTime = getTimerValue64(); // Start time for FPS computing.
		i++;
		goto l_ReadFile;
	} else {
		_FSM_state = my_state_ReadInit;
		goto finished;
	}
l_SendData:
	if (isSchedulable_sendData_launch()) {
		int stop = 0;
		if (1 > SIZE_O - index_O + source_O->read_inds[0]) {
			stop = 1;
		}
		if (stop != 0) {
			_FSM_state = my_state_SendData;
			goto finished;
		}
		sendData_launch();
		i++;
		goto l_SendData;
	} else if (isSchedulable_sendData_done()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_SendData;
			goto finished;
		}
		sendData_done();
		i++;
		goto l_ReadFile;
	} else {
		_FSM_state = my_state_SendData;
		goto finished;
	}
l_SendLastData:
	if (isSchedulable_sendData_launch()) {
		int stop = 0;
		if (1 > SIZE_O - index_O + source_O->read_inds[0]) {
			stop = 1;
		}
		if (stop != 0) {
			_FSM_state = my_state_SendLastData;
			goto finished;
		}
		sendData_launch();
		i++;
		goto l_SendLastData;
	} else if (isSchedulable_sendData_done()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_SendLastData;
			goto finished;
		}
		sendData_done();
		i++;
		goto l_ReadFileDone;
	} else {
		_FSM_state = my_state_SendLastData;
		goto finished;
	}
finished:
//	if(i>0){	// At least one action was executed.
//		// Copying output tokens into an extra buffer for debugging communications.
//		copyOutputTokens(source_O_ix, (u8*)tokens_O, start_index_O, index_O, sizeof(u8));
//	}

	write_end_O();
	return i;
}
