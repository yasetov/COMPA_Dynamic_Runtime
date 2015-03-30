	// Source file is "L/Research/src/ch/epfl/mpeg4/part2/parser/BlockExpand.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_RUN;
	static unsigned int numTokens_RUN;
	#define SIZE_RUN 512
	#define tokens_RUN 	decoder_parser_blkexp_RUN->contents
	
	
	static unsigned int index_VALUE;
	static unsigned int numTokens_VALUE;
	#define SIZE_VALUE 512
	#define tokens_VALUE 	decoder_parser_blkexp_VALUE->contents
	
	
	static unsigned int index_LAST;
	static unsigned int numTokens_LAST;
	#define SIZE_LAST 512
	#define tokens_LAST 	decoder_parser_blkexp_LAST->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_QFS;
	static unsigned int numFree_QFS;
	#define NUM_READERS_QFS 1
	#define SIZE_QFS 512
	#define tokens_QFS decoder_parser_blkexp_QFS->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	static i8 count;
	static i16 qfs[64];
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_run,
		my_state_start
	};

	static char *stateNames[] = {
		"run",
		"start"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_RUN() {
		index_RUN = decoder_parser_blkexp_RUN->read_inds[0];
		numTokens_RUN = index_RUN + fifo_i8_get_num_tokens(decoder_parser_blkexp_RUN, 0);
	}
	
	static void read_end_RUN() {
		decoder_parser_blkexp_RUN->read_inds[0] = index_RUN;
	}
	static void read_VALUE() {
		index_VALUE = decoder_parser_blkexp_VALUE->read_inds[0];
		numTokens_VALUE = index_VALUE + fifo_i16_get_num_tokens(decoder_parser_blkexp_VALUE, 0);
	}
	
	static void read_end_VALUE() {
		decoder_parser_blkexp_VALUE->read_inds[0] = index_VALUE;
	}
	static void read_LAST() {
		index_LAST = decoder_parser_blkexp_LAST->read_inds[0];
		numTokens_LAST = index_LAST + fifo_i32_get_num_tokens(decoder_parser_blkexp_LAST, 0);
	}
	
	static void read_end_LAST() {
		decoder_parser_blkexp_LAST->read_inds[0] = index_LAST;
	}

	static void write_QFS() {
		index_QFS = (*decoder_parser_blkexp_QFS->write_ind);
		numFree_QFS = index_QFS + fifo_i16_get_room(decoder_parser_blkexp_QFS, NUM_READERS_QFS);
	}
	
	static void write_end_QFS() {
		(*decoder_parser_blkexp_QFS->write_ind) = index_QFS;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_start() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void start() {
	
		// Compute aligned port indexes
	
		i32 i;
	
		i = 0;
		while (i <= 63) {
			qfs[i] = 0;
			i = i + 1;
		}
		count = 0;
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_last() {
		i32 result;
		i32 last;
	
		last = tokens_LAST[(index_LAST + (0)) % SIZE_LAST];
		result = last;
		return result;
	}
	
	static void last() {
	
		// Compute aligned port indexes
		i32 index_aligned_RUN = index_RUN % SIZE_RUN;
		i32 index_aligned_VALUE = index_VALUE % SIZE_VALUE;
		i32 index_aligned_LAST = index_LAST % SIZE_LAST;
		i32 index_aligned_QFS = index_QFS % SIZE_QFS;
	
		i8 run;
		i16 value;
		i32 last;
		i8 local_count;
		i32 idx_qfs;
		i16 local_qfs;
	
		run = tokens_RUN[(index_RUN + (0)) % SIZE_RUN];
		value = tokens_VALUE[(index_VALUE + (0)) % SIZE_VALUE];
		last = tokens_LAST[(index_LAST + (0)) % SIZE_LAST];
		local_count = count;
		qfs[local_count + run] = value;
		idx_qfs = 0;
		while (idx_qfs < 64) {
			local_qfs = qfs[idx_qfs];
			tokens_QFS[(index_QFS + (idx_qfs)) % SIZE_QFS] = local_qfs;
			idx_qfs = idx_qfs + 1;
		}
	
		// Update ports indexes
		index_RUN += 1;
		index_VALUE += 1;
		index_LAST += 1;
		index_QFS += 64;
		
		read_end_RUN();
		read_end_VALUE();
		read_end_LAST();
		write_end_QFS();
	}
	static i32 isSchedulable_run() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void run() {
	
		// Compute aligned port indexes
		i32 index_aligned_RUN = index_RUN % SIZE_RUN;
		i32 index_aligned_VALUE = index_VALUE % SIZE_VALUE;
		i32 index_aligned_LAST = index_LAST % SIZE_LAST;
	
		i8 run;
		i16 value;
		i32 last;
		i8 local_count;
	
		run = tokens_RUN[(index_RUN + (0)) % SIZE_RUN];
		value = tokens_VALUE[(index_VALUE + (0)) % SIZE_VALUE];
		last = tokens_LAST[(index_LAST + (0)) % SIZE_LAST];
		local_count = count;
		qfs[local_count + run] = value;
		local_count = count;
		count = local_count + run + 1;
	
		// Update ports indexes
		index_RUN += 1;
		index_VALUE += 1;
		index_LAST += 1;
		
		read_end_RUN();
		read_end_VALUE();
		read_end_LAST();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_parser_blkexp_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_start;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_parser_blkexp_scheduler() {
		int i = 0;
		if(needInit){
			decoder_parser_blkexp_initialize();
			needInit = 0;
		}
	
		read_RUN();
		read_VALUE();
		read_LAST();
		write_QFS();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_run:
			goto l_run;
		case my_state_start:
			goto l_start;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_run:
			if (numTokens_RUN - index_RUN >= 1 && numTokens_VALUE - index_VALUE >= 1 && numTokens_LAST - index_LAST >= 1 && isSchedulable_last()) {
				int stop = 0;
				if (64 > SIZE_QFS - index_QFS + decoder_parser_blkexp_QFS->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_run;
					goto finished;
				}
				last();
				i++;
				goto l_start;
			} else if (numTokens_RUN - index_RUN >= 1 && numTokens_VALUE - index_VALUE >= 1 && numTokens_LAST - index_LAST >= 1 && isSchedulable_run()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_run;
					goto finished;
				}
				run();
				i++;
				goto l_run;
			} else {
				_FSM_state = my_state_run;
				goto finished;
			}
		l_start:
			if (isSchedulable_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_start;
					goto finished;
				}
				start();
				i++;
				goto l_run;
			} else {
				_FSM_state = my_state_start;
				goto finished;
			}
	finished:
		
		read_end_RUN();
		read_end_VALUE();
		read_end_LAST();
		
		write_end_QFS();
		
		
		return i;
	}
	
