	// Source file is "L/Research/src/com/ericsson/mpeg4/part2/sp/Algo_Interpolation_halfpel.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_RD;
	static unsigned int numTokens_RD;
	#define SIZE_RD 512
	#define tokens_RD 	decoder_motion_interpolation_RD->contents
	
	
	static unsigned int index_halfpel;
	static unsigned int numTokens_halfpel;
	#define SIZE_halfpel 512
	#define tokens_halfpel 	decoder_motion_interpolation_halfpel->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_MOT;
	static unsigned int numFree_MOT;
	#define NUM_READERS_MOT 1
	#define SIZE_MOT 512
	#define tokens_MOT decoder_motion_interpolation_MOT->contents



	
	
	
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_RD() {
		index_RD = decoder_motion_interpolation_RD->read_inds[0];
		numTokens_RD = index_RD + fifo_i16_get_num_tokens(decoder_motion_interpolation_RD, 0);
	}
	
	static void read_end_RD() {
		decoder_motion_interpolation_RD->read_inds[0] = index_RD;
	}
	static void read_halfpel() {
		index_halfpel = decoder_motion_interpolation_halfpel->read_inds[0];
		numTokens_halfpel = index_halfpel + fifo_i8_get_num_tokens(decoder_motion_interpolation_halfpel, 0);
	}
	
	static void read_end_halfpel() {
		decoder_motion_interpolation_halfpel->read_inds[0] = index_halfpel;
	}

	static void write_MOT() {
		index_MOT = (*decoder_motion_interpolation_MOT->write_ind);
		numFree_MOT = index_MOT + fifo_i16_get_room(decoder_motion_interpolation_MOT, NUM_READERS_MOT);
	}
	
	static void write_end_MOT() {
		(*decoder_motion_interpolation_MOT->write_ind) = index_MOT;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_none() {
		i32 result;
		i8 flags;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		result = flags >> 1 == 0;
		return result;
	}
	
	static void none() {
	
		// Compute aligned port indexes
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
		i32 index_aligned_RD = index_RD % SIZE_RD;
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
	
		i8 flags;
		i32 j;
		i32 k;
		i16 tmp_RD;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		j = 0;
		while (j <= 7) {
			k = 0;
			while (k <= 7) {
				tmp_RD = tokens_RD[(index_RD + (9 * j + k)) % SIZE_RD];
				tokens_MOT[(index_MOT + (j * 8 + k)) % SIZE_MOT] = tmp_RD;
				k = k + 1;
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_halfpel += 1;
		index_RD += 81;
		index_MOT += 64;
		
		read_end_halfpel();
		read_end_RD();
		write_end_MOT();
	}
	static i32 isSchedulable_row() {
		i32 result;
		i8 flags;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		result = flags >> 1 == 1;
		return result;
	}
	
	static void row() {
	
		// Compute aligned port indexes
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
		i32 index_aligned_RD = index_RD % SIZE_RD;
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
	
		i8 flags;
		i32 round;
		i32 j;
		i32 k;
		i16 tmp_RD;
		i16 tmp_RD0;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		round = 1 - (flags & 1);
		j = 0;
		while (j <= 7) {
			k = 0;
			while (k <= 7) {
				tmp_RD = tokens_RD[(index_RD + (9 * j + k)) % SIZE_RD];
				tmp_RD0 = tokens_RD[(index_RD + (9 * (j + 1) + k)) % SIZE_RD];
				tokens_MOT[(index_MOT + (j * 8 + k)) % SIZE_MOT] = (tmp_RD + tmp_RD0 + round) >> 1;
				k = k + 1;
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_halfpel += 1;
		index_RD += 81;
		index_MOT += 64;
		
		read_end_halfpel();
		read_end_RD();
		write_end_MOT();
	}
	static i32 isSchedulable_col() {
		i32 result;
		i8 flags;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		result = flags >> 1 == 2;
		return result;
	}
	
	static void col() {
	
		// Compute aligned port indexes
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
		i32 index_aligned_RD = index_RD % SIZE_RD;
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
	
		i8 flags;
		i32 round;
		i32 j;
		i32 k;
		i16 tmp_RD;
		i16 tmp_RD0;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		round = 1 - (flags & 1);
		j = 0;
		while (j <= 7) {
			k = 0;
			while (k <= 7) {
				tmp_RD = tokens_RD[(index_RD + (9 * j + k)) % SIZE_RD];
				tmp_RD0 = tokens_RD[(index_RD + (9 * j + k + 1)) % SIZE_RD];
				tokens_MOT[(index_MOT + (j * 8 + k)) % SIZE_MOT] = (tmp_RD + tmp_RD0 + round) >> 1;
				k = k + 1;
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_halfpel += 1;
		index_RD += 81;
		index_MOT += 64;
		
		read_end_halfpel();
		read_end_RD();
		write_end_MOT();
	}
	static i32 isSchedulable_both() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void both() {
	
		// Compute aligned port indexes
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
		i32 index_aligned_RD = index_RD % SIZE_RD;
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
	
		i8 flags;
		i32 round;
		i32 j;
		i32 k;
		i16 tmp_RD;
		i16 tmp_RD0;
		i16 tmp_RD1;
		i16 tmp_RD2;
	
		flags = tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel];
		round = 2 - (flags & 1);
		j = 0;
		while (j <= 7) {
			k = 0;
			while (k <= 7) {
				tmp_RD = tokens_RD[(index_RD + (9 * j + k)) % SIZE_RD];
				tmp_RD0 = tokens_RD[(index_RD + (9 * j + k + 1)) % SIZE_RD];
				tmp_RD1 = tokens_RD[(index_RD + (9 * (j + 1) + k)) % SIZE_RD];
				tmp_RD2 = tokens_RD[(index_RD + (9 * (j + 1) + k + 1)) % SIZE_RD];
				tokens_MOT[(index_MOT + (j * 8 + k)) % SIZE_MOT] = (tmp_RD + tmp_RD0 + tmp_RD1 + tmp_RD2 + round) >> 2;
				k = k + 1;
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_halfpel += 1;
		index_RD += 81;
		index_MOT += 64;
		
		read_end_halfpel();
		read_end_RD();
		write_end_MOT();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_motion_interpolation_initialize() {
		int i = 0;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	int decoder_motion_interpolation_scheduler() {
		int i = 0;
		if(needInit){
			decoder_motion_interpolation_initialize();
			needInit = 0;
		}
		read_RD();
		read_halfpel();
		write_MOT();
		
	
	
		while (1) {
			// Action loop
			if (numTokens_halfpel - index_halfpel >= 1 && numTokens_RD - index_RD >= 81 && isSchedulable_none()) {
				int stop = 0;
				if (64 > SIZE_MOT - index_MOT + decoder_motion_interpolation_MOT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				none();
				i++;
			} else if (numTokens_halfpel - index_halfpel >= 1 && numTokens_RD - index_RD >= 81 && isSchedulable_col()) {
				int stop = 0;
				if (64 > SIZE_MOT - index_MOT + decoder_motion_interpolation_MOT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				col();
				i++;
			} else if (numTokens_halfpel - index_halfpel >= 1 && numTokens_RD - index_RD >= 81 && isSchedulable_row()) {
				int stop = 0;
				if (64 > SIZE_MOT - index_MOT + decoder_motion_interpolation_MOT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				row();
				i++;
			} else if (numTokens_halfpel - index_halfpel >= 1 && numTokens_RD - index_RD >= 81 && isSchedulable_both()) {
				int stop = 0;
				if (64 > SIZE_MOT - index_MOT + decoder_motion_interpolation_MOT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				both();
				i++;
			} else {
				goto finished;
			}
		}
		
	finished:
	
		read_end_RD();
		read_end_halfpel();
		write_end_MOT();
		
		
		return i;
	}
	
