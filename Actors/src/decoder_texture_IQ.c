	// Source file is "L/Research/src/fr/irisa/mpeg4/part2/sp/texture/Algo_IQ_QSAbdQmatrixMp4vOrH263Scaler.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_DC;
	static unsigned int numTokens_DC;
	#define SIZE_DC 512
	#define tokens_DC 	decoder_texture_IQ_DC->contents
	
	
	static unsigned int index_AC;
	static unsigned int numTokens_AC;
	#define SIZE_AC 512
	#define tokens_AC 	decoder_texture_IQ_AC->contents
	
	
	static unsigned int index_QP;
	static unsigned int numTokens_QP;
	#define SIZE_QP 512
	#define tokens_QP 	decoder_texture_IQ_QP->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_OUT;
	static unsigned int numFree_OUT;
	#define NUM_READERS_OUT 1
	#define SIZE_OUT 512
	#define tokens_OUT decoder_texture_IQ_OUT->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	static i8 quant;
	static i8 round;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_ac,
		my_state_start
	};

	static char *stateNames[] = {
		"ac",
		"start"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_DC() {
		index_DC = decoder_texture_IQ_DC->read_inds[0];
		numTokens_DC = index_DC + fifo_i16_get_num_tokens(decoder_texture_IQ_DC, 0);
	}
	
	static void read_end_DC() {
		decoder_texture_IQ_DC->read_inds[0] = index_DC;
	}
	static void read_AC() {
		index_AC = decoder_texture_IQ_AC->read_inds[0];
		numTokens_AC = index_AC + fifo_i16_get_num_tokens(decoder_texture_IQ_AC, 0);
	}
	
	static void read_end_AC() {
		decoder_texture_IQ_AC->read_inds[0] = index_AC;
	}
	static void read_QP() {
		index_QP = decoder_texture_IQ_QP->read_inds[0];
		numTokens_QP = index_QP + fifo_i8_get_num_tokens(decoder_texture_IQ_QP, 0);
	}
	
	static void read_end_QP() {
		decoder_texture_IQ_QP->read_inds[0] = index_QP;
	}

	static void write_OUT() {
		index_OUT = (*decoder_texture_IQ_OUT->write_ind);
		numFree_OUT = index_OUT + fifo_i16_get_room(decoder_texture_IQ_OUT, NUM_READERS_OUT);
	}
	
	static void write_end_OUT() {
		(*decoder_texture_IQ_OUT->write_ind) = index_OUT;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i32 Math_abs(i32 x);
	static i32 saturate(i32 x);

	static i32 Math_abs(i32 x) {
		i32 tmp_if;
	
		if (x > 0) {
			tmp_if = x;
		} else {
			tmp_if = -x;
		}
		return tmp_if;
	}
	static i32 saturate(i32 x) {
		i32 minus;
		i32 plus;
		i32 tmp_if;
	
		minus = x < -2048;
		plus = x > 2047;
		if (minus) {
			tmp_if = -2048;
		} else {
			if (plus) {
				tmp_if = 2047;
			} else {
				tmp_if = x;
			}
		}
		return tmp_if;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_get_qp() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_qp() {
	
		// Compute aligned port indexes
		i32 index_aligned_QP = index_QP % SIZE_QP;
		i32 index_aligned_DC = index_DC % SIZE_DC;
		i32 index_aligned_OUT = index_OUT % SIZE_OUT;
	
		i8 q;
		i16 i;
	
		q = tokens_QP[(index_QP + (0)) % SIZE_QP];
		i = tokens_DC[(index_DC + (0)) % SIZE_DC];
		quant = q;
		round = q & 1 ^ 1;
		tokens_OUT[(index_OUT + (0)) % SIZE_OUT] = i;
	
		// Update ports indexes
		index_QP += 1;
		index_DC += 1;
		index_OUT += 1;
		
		read_end_QP();
		read_end_DC();
		write_end_OUT();
	}
	static i32 isSchedulable_ac() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void ac() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC = index_AC % SIZE_AC;
		i32 index_aligned_OUT = index_OUT % SIZE_OUT;
	
		i16 iq;
		i16 ac_i;
		i32 j;
		i8 local_quant;
		i32 tmp_abs;
		i8 local_round;
		i32 tmp_saturate;
		i32 tmp_saturate0;
	
		j = 0;
		while (j <= 62) {
			ac_i = tokens_AC[(index_AC + (j)) % SIZE_AC];
			local_quant = quant;
			tmp_abs = Math_abs(ac_i);
			local_round = round;
			iq = local_quant * ((tmp_abs << 1) + 1) - local_round;
			if (ac_i == 0) {
				tokens_OUT[(index_OUT + (j)) % SIZE_OUT] = 0;
			} else {
				if (ac_i < 0) {
					tmp_saturate = saturate(-iq);
					tokens_OUT[(index_OUT + (j)) % SIZE_OUT] = tmp_saturate;
				} else {
					tmp_saturate0 = saturate(iq);
					tokens_OUT[(index_OUT + (j)) % SIZE_OUT] = tmp_saturate0;
				}
			}
			j = j + 1;
		}
	
		// Update ports indexes
		index_AC += 63;
		index_OUT += 63;
		
		read_end_AC();
		write_end_OUT();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_IQ_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_start;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_texture_IQ_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_IQ_initialize();
			needInit = 0;
		}
	
		read_DC();
		read_AC();
		read_QP();
		write_OUT();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_ac:
			goto l_ac;
		case my_state_start:
			goto l_start;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_ac:
			if (numTokens_AC - index_AC >= 63 && isSchedulable_ac()) {
				int stop = 0;
				if (63 > SIZE_OUT - index_OUT + decoder_texture_IQ_OUT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_ac;
					goto finished;
				}
				ac();
				i++;
				goto l_start;
			} else {
				_FSM_state = my_state_ac;
				goto finished;
			}
		l_start:
			if (numTokens_QP - index_QP >= 1 && numTokens_DC - index_DC >= 1 && isSchedulable_get_qp()) {
				int stop = 0;
				if (1 > SIZE_OUT - index_OUT + decoder_texture_IQ_OUT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_start;
					goto finished;
				}
				get_qp();
				i++;
				goto l_ac;
			} else {
				_FSM_state = my_state_start;
				goto finished;
			}
	finished:
		
		read_end_DC();
		read_end_AC();
		read_end_QP();
		
		write_end_OUT();
		
		
		return i;
	}
	
