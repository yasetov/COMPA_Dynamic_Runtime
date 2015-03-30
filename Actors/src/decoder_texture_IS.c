	// Source file is "L/Research/src/com/ericsson/mpeg4/part2/sp/Algo_IS.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_AC_PRED_DIR;
	static unsigned int numTokens_AC_PRED_DIR;
	#define SIZE_AC_PRED_DIR 512
	#define tokens_AC_PRED_DIR 	decoder_texture_IS_AC_PRED_DIR->contents
	
	
	static unsigned int index_QFS_AC;
	static unsigned int numTokens_QFS_AC;
	#define SIZE_QFS_AC 512
	#define tokens_QFS_AC 	decoder_texture_IS_QFS_AC->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_PQF_AC;
	static unsigned int numFree_PQF_AC;
	#define NUM_READERS_PQF_AC 1
	#define SIZE_PQF_AC 512
	#define tokens_PQF_AC decoder_texture_IS_PQF_AC->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	static const i8 Scanmode[192] = {0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26, 29, 42, 3, 8, 12, 17, 25, 30, 41, 43, 9, 11, 18, 24, 31, 40, 44, 53, 10, 19, 23, 32, 39, 45, 52, 54, 20, 22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56, 59, 61, 35, 36, 48, 49, 57, 58, 62, 63, 0, 4, 6, 20, 22, 36, 38, 52, 1, 5, 7, 21, 23, 37, 39, 53, 2, 8, 19, 24, 34, 40, 50, 54, 3, 9, 18, 25, 35, 41, 51, 55, 10, 17, 26, 30, 42, 46, 56, 60, 11, 16, 27, 31, 43, 47, 57, 61, 12, 15, 28, 32, 44, 48, 58, 62, 13, 14, 29, 33, 45, 49, 59, 63, 0, 1, 2, 3, 10, 11, 12, 13, 4, 5, 8, 9, 17, 16, 15, 14, 6, 7, 19, 18, 26, 27, 28, 29, 20, 21, 24, 25, 30, 31, 32, 33, 22, 23, 34, 35, 42, 43, 44, 45, 36, 37, 40, 41, 46, 47, 48, 49, 38, 39, 50, 51, 56, 57, 58, 59, 52, 53, 54, 55, 60, 61, 62, 63};
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_AC_PRED_DIR() {
		index_AC_PRED_DIR = decoder_texture_IS_AC_PRED_DIR->read_inds[0];
		numTokens_AC_PRED_DIR = index_AC_PRED_DIR + fifo_i8_get_num_tokens(decoder_texture_IS_AC_PRED_DIR, 0);
	}
	
	static void read_end_AC_PRED_DIR() {
		decoder_texture_IS_AC_PRED_DIR->read_inds[0] = index_AC_PRED_DIR;
	}
	static void read_QFS_AC() {
		index_QFS_AC = decoder_texture_IS_QFS_AC->read_inds[0];
		numTokens_QFS_AC = index_QFS_AC + fifo_i16_get_num_tokens(decoder_texture_IS_QFS_AC, 0);
	}
	
	static void read_end_QFS_AC() {
		decoder_texture_IS_QFS_AC->read_inds[0] = index_QFS_AC;
	}

	static void write_PQF_AC() {
		index_PQF_AC = (*decoder_texture_IS_PQF_AC->write_ind);
		numFree_PQF_AC = index_PQF_AC + fifo_i16_get_room(decoder_texture_IS_PQF_AC, NUM_READERS_PQF_AC);
	}
	
	static void write_end_PQF_AC() {
		(*decoder_texture_IS_PQF_AC->write_ind) = index_PQF_AC;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_skip() {
		i32 result;
		i8 i;
	
		i = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		result = i < 0;
		return result;
	}
	
	static void skip() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
	
		i8 i;
	
		i = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
	
		// Update ports indexes
		index_AC_PRED_DIR += 1;
		
		read_end_AC_PRED_DIR();
	}
	static i32 isSchedulable_rw() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void rw() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
		i32 index_aligned_QFS_AC = index_QFS_AC % SIZE_QFS_AC;
		i32 index_aligned_PQF_AC = index_PQF_AC % SIZE_PQF_AC;
	
		i8 dir;
		i32 offset;
		i32 addr;
		i8 tmp_Scanmode;
		i16 tmp_QFS_AC;
	
		dir = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		offset = (dir << 6) + 1;
		addr = 0;
		while (addr <= 62) {
			tmp_Scanmode = Scanmode[offset + addr];
			tmp_QFS_AC = tokens_QFS_AC[(index_QFS_AC + (tmp_Scanmode - 1)) % SIZE_QFS_AC];
			tokens_PQF_AC[(index_PQF_AC + (addr)) % SIZE_PQF_AC] = tmp_QFS_AC;
			addr = addr + 1;
		}
	
		// Update ports indexes
		index_AC_PRED_DIR += 1;
		index_QFS_AC += 63;
		index_PQF_AC += 63;
		
		read_end_AC_PRED_DIR();
		read_end_QFS_AC();
		write_end_PQF_AC();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_IS_initialize() {
		int i = 0;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	int decoder_texture_IS_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_IS_initialize();
			needInit = 0;
		}
		read_AC_PRED_DIR();
		read_QFS_AC();
		write_PQF_AC();
		
	
	
		while (1) {
			// Action loop
			if (numTokens_AC_PRED_DIR - index_AC_PRED_DIR >= 1 && numTokens_QFS_AC - index_QFS_AC >= 63 && isSchedulable_skip()) {
				int stop = 0;
				if (stop != 0) {
					goto finished;
				}
				skip();
				i++;
			} else if (numTokens_AC_PRED_DIR - index_AC_PRED_DIR >= 1 && numTokens_QFS_AC - index_QFS_AC >= 63 && isSchedulable_rw()) {
				int stop = 0;
				if (63 > SIZE_PQF_AC - index_PQF_AC + decoder_texture_IS_PQF_AC->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				rw();
				i++;
			} else {
				goto finished;
			}
		}
		
	finished:
	
		read_end_AC_PRED_DIR();
		read_end_QFS_AC();
		write_end_PQF_AC();
		
		
		return i;
	}
	
