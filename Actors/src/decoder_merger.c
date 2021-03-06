	// Source file is "L/Research/src/com/xilinx/mpeg4/part2/sp/Mgnt_Merger.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_YUV_i;
	static unsigned int numTokens_YUV_i;
	#define SIZE_YUV_i 512
	#define tokens_YUV_i 	decoder_merger_YUV_i->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_YUV;
	static unsigned int numFree_YUV;
	#define NUM_READERS_YUV 1
	#define SIZE_YUV 512
	#define tokens_YUV decoder_merger_YUV->contents



	
	
	
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_U,
		my_state_V,
		my_state_Y
	};

	static char *stateNames[] = {
		"U",
		"V",
		"Y"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_YUV_i() {
		index_YUV_i = decoder_merger_YUV_i->read_inds[1];
		numTokens_YUV_i = index_YUV_i + fifo_i16_get_num_tokens(decoder_merger_YUV_i, 1);
	}
	
	static void read_end_YUV_i() {
		decoder_merger_YUV_i->read_inds[1] = index_YUV_i;
	}

	static void write_YUV() {
		index_YUV = (*decoder_merger_YUV->write_ind);
		numFree_YUV = index_YUV + fifo_u8_get_room(decoder_merger_YUV, NUM_READERS_YUV);
	}
	
	static void write_end_YUV() {
		(*decoder_merger_YUV->write_ind) = index_YUV;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_Y() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void Y() {
	
		// Compute aligned port indexes
		i32 index_aligned_YUV_i = index_YUV_i % SIZE_YUV_i;
		i32 index_aligned_YUV = index_YUV % SIZE_YUV;
	
		i32 j;
		i32 i;
		i16 tmp_YUV_i;
		i16 tmp_YUV_i0;
		i16 tmp_YUV_i1;
		i16 tmp_YUV_i2;
	
		j = 0;
		while (j <= 7) {
			i = 0;
			while (i <= 7) {
				tmp_YUV_i = tokens_YUV_i[(index_YUV_i + (i + j * 8)) % SIZE_YUV_i];
				tokens_YUV[(index_YUV + (i + j * 16)) % SIZE_YUV] = tmp_YUV_i;
				i = i + 1;
			}
			j = j + 1;
		}
		j = 0;
		while (j <= 7) {
			i = 0;
			while (i <= 7) {
				tmp_YUV_i0 = tokens_YUV_i[(index_YUV_i + (i + 64 + j * 8)) % SIZE_YUV_i];
				tokens_YUV[(index_YUV + (i + 8 + j * 16)) % SIZE_YUV] = tmp_YUV_i0;
				i = i + 1;
			}
			j = j + 1;
		}
		j = 0;
		while (j <= 7) {
			i = 0;
			while (i <= 7) {
				tmp_YUV_i1 = tokens_YUV_i[(index_YUV_i + (i + 2 * 64 + j * 8)) % SIZE_YUV_i];
				tokens_YUV[(index_YUV + (i + (j + 8) * 16)) % SIZE_YUV] = tmp_YUV_i1;
				i = i + 1;
			}
			j = j + 1;
		}
		j = 0;
		while (j <= 7) {
			i = 0;
			while (i <= 7) {
				tmp_YUV_i2 = tokens_YUV_i[(index_YUV_i + (i + 3 * 64 + j * 8)) % SIZE_YUV_i];
				tokens_YUV[(index_YUV + (i + 8 + (j + 8) * 16)) % SIZE_YUV] = tmp_YUV_i2;
				i = i + 1;
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_YUV_i += 256;
		index_YUV += 256;
		
		read_end_YUV_i();
		write_end_YUV();
	}
	static i32 isSchedulable_U() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void U() {
	
		// Compute aligned port indexes
		i32 index_aligned_YUV_i = index_YUV_i % SIZE_YUV_i;
		i32 index_aligned_YUV = index_YUV % SIZE_YUV;
	
		i32 idx_YUV_i;
		u8 local_YUV_i;
	
		idx_YUV_i = 0;
		while (idx_YUV_i < 64) {
			local_YUV_i = tokens_YUV_i[(index_YUV_i + (idx_YUV_i)) % SIZE_YUV_i];
			tokens_YUV[(index_YUV + (idx_YUV_i)) % SIZE_YUV] = local_YUV_i;
			idx_YUV_i = idx_YUV_i + 1;
		}
	
		// Update ports indexes
		index_YUV_i += 64;
		index_YUV += 64;
		
		read_end_YUV_i();
		write_end_YUV();
	}
	static i32 isSchedulable_V() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void V() {
	
		// Compute aligned port indexes
		i32 index_aligned_YUV_i = index_YUV_i % SIZE_YUV_i;
		i32 index_aligned_YUV = index_YUV % SIZE_YUV;
	
		i32 idx_YUV_i;
		u8 local_YUV_i;
	
		idx_YUV_i = 0;
		while (idx_YUV_i < 64) {
			local_YUV_i = tokens_YUV_i[(index_YUV_i + (idx_YUV_i)) % SIZE_YUV_i];
			tokens_YUV[(index_YUV + (idx_YUV_i)) % SIZE_YUV] = local_YUV_i;
			idx_YUV_i = idx_YUV_i + 1;
		}
	
		// Update ports indexes
		index_YUV_i += 64;
		index_YUV += 64;
		
		read_end_YUV_i();
		write_end_YUV();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_merger_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_Y;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_merger_scheduler() {
		int i = 0;
		if(needInit){
			decoder_merger_initialize();
			needInit = 0;
		}
	
		read_YUV_i();
		write_YUV();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_U:
			goto l_U;
		case my_state_V:
			goto l_V;
		case my_state_Y:
			goto l_Y;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_U:
			if (numTokens_YUV_i - index_YUV_i >= 64 && isSchedulable_U()) {
				int stop = 0;
				if (64 > SIZE_YUV - index_YUV + decoder_merger_YUV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_U;
					goto finished;
				}
				U();
				i++;
				goto l_V;
			} else {
				_FSM_state = my_state_U;
				goto finished;
			}
		l_V:
			if (numTokens_YUV_i - index_YUV_i >= 64 && isSchedulable_V()) {
				int stop = 0;
				if (64 > SIZE_YUV - index_YUV + decoder_merger_YUV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_V;
					goto finished;
				}
				V();
				i++;
				goto l_Y;
			} else {
				_FSM_state = my_state_V;
				goto finished;
			}
		l_Y:
			if (numTokens_YUV_i - index_YUV_i >= 256 && isSchedulable_Y()) {
				int stop = 0;
				if (256 > SIZE_YUV - index_YUV + decoder_merger_YUV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_Y;
					goto finished;
				}
				Y();
				i++;
				goto l_U;
			} else {
				_FSM_state = my_state_Y;
				goto finished;
			}
	finished:
		
		read_end_YUV_i();
		
		write_end_YUV();
		
		
		return i;
	}
	
