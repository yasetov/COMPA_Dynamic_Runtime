	// Source file is "L/RVC/src/org/sc29/wg11/mpeg4/part2/sp/texture/Mgnt_DCSplit.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_IN;
	static unsigned int numTokens_IN;
	#define SIZE_IN 512
	#define tokens_IN 	decoder_texture_DCsplit_IN->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_DC;
	static unsigned int numFree_DC;
	#define NUM_READERS_DC 1
	#define SIZE_DC 512
	#define tokens_DC decoder_texture_DCsplit_DC->contents
	static unsigned int index_AC;
	static unsigned int numFree_AC;
	#define NUM_READERS_AC 1
	#define SIZE_AC 512
	#define tokens_AC decoder_texture_DCsplit_AC->contents



	
	
	
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_IN() {
		index_IN = decoder_texture_DCsplit_IN->read_inds[0];
		numTokens_IN = index_IN + fifo_i16_get_num_tokens(decoder_texture_DCsplit_IN, 0);
	}
	
	static void read_end_IN() {
		decoder_texture_DCsplit_IN->read_inds[0] = index_IN;
	}

	static void write_DC() {
		index_DC = (*decoder_texture_DCsplit_DC->write_ind);
		numFree_DC = index_DC + fifo_i16_get_room(decoder_texture_DCsplit_DC, NUM_READERS_DC);
	}
	
	static void write_end_DC() {
		(*decoder_texture_DCsplit_DC->write_ind) = index_DC;
	}
	static void write_AC() {
		index_AC = (*decoder_texture_DCsplit_AC->write_ind);
		numFree_AC = index_AC + fifo_i16_get_room(decoder_texture_DCsplit_AC, NUM_READERS_AC);
	}
	
	static void write_end_AC() {
		(*decoder_texture_DCsplit_AC->write_ind) = index_AC;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_untagged_0() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void untagged_0() {
	
		// Compute aligned port indexes
		i32 index_aligned_IN = index_IN % SIZE_IN;
		i32 index_aligned_DC = index_DC % SIZE_DC;
		i32 index_aligned_AC = index_AC % SIZE_AC;
	
		i16 tmp_IN;
		i32 i;
		i16 tmp_IN0;
	
		tmp_IN = tokens_IN[(index_IN + (0)) % SIZE_IN];
		tokens_DC[(index_DC + (0)) % SIZE_DC] = tmp_IN;
		i = 1;
		while (i <= 63) {
			tmp_IN0 = tokens_IN[(index_IN + (i)) % SIZE_IN];
			tokens_AC[(index_AC + (i - 1)) % SIZE_AC] = tmp_IN0;
			i = i + 1;
		}
	
		// Update ports indexes
		index_IN += 64;
		index_DC += 1;
		index_AC += 63;
		
		read_end_IN();
		write_end_DC();
		write_end_AC();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_DCsplit_initialize() {
		int i = 0;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	int decoder_texture_DCsplit_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_DCsplit_initialize();
			needInit = 0;
		}
		read_IN();
		write_DC();
		write_AC();
		
	
	
		while (1) {
			// Action loop
			if (numTokens_IN - index_IN >= 64 && isSchedulable_untagged_0()) {
				int stop = 0;
				if (1 > SIZE_DC - index_DC + decoder_texture_DCsplit_DC->read_inds[0]) {
					stop = 1;
				}
				if (63 > SIZE_AC - index_AC + decoder_texture_DCsplit_AC->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					goto finished;
				}
				untagged_0();
				i++;
			} else {
				goto finished;
			}
		}
		
	finished:
	
		read_end_IN();
		write_end_DC();
		write_end_AC();
		
		
		return i;
	}
	
