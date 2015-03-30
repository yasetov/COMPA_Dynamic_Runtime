	// Source file is "L/Research/src/fi/oulu/ee/mpeg4/part2/sp/Algo_IAP_multi.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_PQF_AC;
	static unsigned int numTokens_PQF_AC;
	#define SIZE_PQF_AC 512
	#define tokens_PQF_AC 	decoder_texture_IAP_PQF_AC->contents
	
	
	static unsigned int index_PTR;
	static unsigned int numTokens_PTR;
	#define SIZE_PTR 512
	#define tokens_PTR 	decoder_texture_IAP_PTR->contents
	
	
	static unsigned int index_AC_PRED_DIR;
	static unsigned int numTokens_AC_PRED_DIR;
	#define SIZE_AC_PRED_DIR 512
	#define tokens_AC_PRED_DIR 	decoder_texture_IAP_AC_PRED_DIR->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_QF_AC;
	static unsigned int numFree_QF_AC;
	#define NUM_READERS_QF_AC 1
	#define SIZE_QF_AC 512
	#define tokens_QF_AC decoder_texture_IAP_QF_AC->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define BUF_SIZE 112
	static i16 ptr;
	static i16 pred_ptr;
	static i16 buf[1792];
	static i8 comp;
	static i32 top;
	static i32 acpred_flag;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_advance,
		my_state_pred,
		my_state_start
	};

	static char *stateNames[] = {
		"advance",
		"pred",
		"start"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_PQF_AC() {
		index_PQF_AC = decoder_texture_IAP_PQF_AC->read_inds[0];
		numTokens_PQF_AC = index_PQF_AC + fifo_i16_get_num_tokens(decoder_texture_IAP_PQF_AC, 0);
	}
	
	static void read_end_PQF_AC() {
		decoder_texture_IAP_PQF_AC->read_inds[0] = index_PQF_AC;
	}
	static void read_PTR() {
		index_PTR = decoder_texture_IAP_PTR->read_inds[0];
		numTokens_PTR = index_PTR + fifo_i16_get_num_tokens(decoder_texture_IAP_PTR, 0);
	}
	
	static void read_end_PTR() {
		decoder_texture_IAP_PTR->read_inds[0] = index_PTR;
	}
	static void read_AC_PRED_DIR() {
		index_AC_PRED_DIR = decoder_texture_IAP_AC_PRED_DIR->read_inds[0];
		numTokens_AC_PRED_DIR = index_AC_PRED_DIR + fifo_i8_get_num_tokens(decoder_texture_IAP_AC_PRED_DIR, 0);
	}
	
	static void read_end_AC_PRED_DIR() {
		decoder_texture_IAP_AC_PRED_DIR->read_inds[0] = index_AC_PRED_DIR;
	}

	static void write_QF_AC() {
		index_QF_AC = (*decoder_texture_IAP_QF_AC->write_ind);
		numFree_QF_AC = index_QF_AC + fifo_i16_get_room(decoder_texture_IAP_QF_AC, NUM_READERS_QF_AC);
	}
	
	static void write_end_QF_AC() {
		(*decoder_texture_IAP_QF_AC->write_ind) = index_QF_AC;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_newvop() {
		i32 result;
		i8 s;
	
		s = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		result = s == -2;
		return result;
	}
	
	static void newvop() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
	
		i8 s;
	
		s = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		comp = 0;
		ptr = 8;
	
		// Update ports indexes
		index_AC_PRED_DIR += 1;
		
		read_end_AC_PRED_DIR();
	}
	static i32 isSchedulable_skip() {
		i32 result;
		i8 s;
	
		s = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		result = s < 0;
		return result;
	}
	
	static void skip() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
	
		i8 s;
	
		s = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
	
		// Update ports indexes
		index_AC_PRED_DIR += 1;
		
		read_end_AC_PRED_DIR();
	}
	static i32 isSchedulable_start() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void start() {
	
		// Compute aligned port indexes
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
		i32 index_aligned_PTR = index_PTR % SIZE_PTR;
	
		i8 s;
		i16 p;
	
		s = tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR];
		p = tokens_PTR[(index_PTR + (0)) % SIZE_PTR];
		pred_ptr = p;
		top = s == 2;
		acpred_flag = s != 0;
	
		// Update ports indexes
		index_AC_PRED_DIR += 1;
		index_PTR += 1;
		
		read_end_AC_PRED_DIR();
		read_end_PTR();
	}
	static i32 isSchedulable_advance() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void advance() {
	
		// Compute aligned port indexes
	
		i8 local_comp;
		i16 local_ptr;
		i32 local_BUF_SIZE;
	
		local_comp = comp;
		comp = local_comp + 1;
		local_comp = comp;
		if (local_comp == 6) {
			comp = 0;
			local_ptr = ptr;
			if (local_ptr == 8) {
				local_BUF_SIZE = BUF_SIZE;
				ptr = local_BUF_SIZE - 8;
			} else {
				local_ptr = ptr;
				ptr = local_ptr - 8;
			}
		}
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_copy() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void copy() {
	
		// Compute aligned port indexes
		i32 index_aligned_PQF_AC = index_PQF_AC % SIZE_PQF_AC;
		i32 index_aligned_QF_AC = index_QF_AC % SIZE_QF_AC;
	
		i8 v;
		i8 h;
		i32 top_edge;
		i32 left_edge;
		i8 index_replaced;
		i32 count;
		i32 i;
		i32 local_acpred_flag;
		i32 local_top;
		i16 tmp_PQF_AC;
		i16 local_pred_ptr;
		i16 tmp_buf;
		i16 tmp_PQF_AC0;
		i16 local_ptr;
		i8 local_comp;
		i16 tmp_pred;
	
		i = 0;
		while (i <= 62) {
			count = i + 1;
			v = count & 7;
			h = count >> 3 & 7;
			top_edge = h == 0;
			left_edge = v == 0;
			if (top_edge) {
				index_replaced = v;
			} else {
				index_replaced = h | 8;
			}
			local_acpred_flag = acpred_flag;
			local_top = top;
			local_top = top;
			if (local_acpred_flag && (local_top && top_edge || !local_top && left_edge)) {
				tmp_PQF_AC = tokens_PQF_AC[(index_PQF_AC + (i)) % SIZE_PQF_AC];
				local_pred_ptr = pred_ptr;
				tmp_buf = buf[local_pred_ptr << 4 | index_replaced];
				tokens_QF_AC[(index_QF_AC + (i)) % SIZE_QF_AC] = tmp_PQF_AC + tmp_buf;
			} else {
				tmp_PQF_AC0 = tokens_PQF_AC[(index_PQF_AC + (i)) % SIZE_PQF_AC];
				tokens_QF_AC[(index_QF_AC + (i)) % SIZE_QF_AC] = tmp_PQF_AC0;
			}
			if (left_edge || top_edge) {
				local_ptr = ptr;
				local_comp = comp;
				tmp_pred = tokens_QF_AC[(index_QF_AC + (i)) % SIZE_QF_AC];
				buf[(local_ptr | local_comp) << 4 | index_replaced] = tmp_pred;
			}
			i = i + 1;
		}
	
		// Update ports indexes
		index_PQF_AC += 63;
		index_QF_AC += 63;
		
		read_end_PQF_AC();
		write_end_QF_AC();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_IAP_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_start;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_texture_IAP_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_IAP_initialize();
			needInit = 0;
		}
	
		read_PQF_AC();
		read_PTR();
		read_AC_PRED_DIR();
		write_QF_AC();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_advance:
			goto l_advance;
		case my_state_pred:
			goto l_pred;
		case my_state_start:
			goto l_start;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_advance:
			if (isSchedulable_advance()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_advance;
					goto finished;
				}
				advance();
				i++;
				goto l_start;
			} else {
				_FSM_state = my_state_advance;
				goto finished;
			}
		l_pred:
			if (numTokens_PQF_AC - index_PQF_AC >= 63 && isSchedulable_copy()) {
				int stop = 0;
				if (63 > SIZE_QF_AC - index_QF_AC + decoder_texture_IAP_QF_AC->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_pred;
					goto finished;
				}
				copy();
				i++;
				goto l_advance;
			} else {
				_FSM_state = my_state_pred;
				goto finished;
			}
		l_start:
			if (numTokens_AC_PRED_DIR - index_AC_PRED_DIR >= 1 && isSchedulable_newvop()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_start;
					goto finished;
				}
				newvop();
				i++;
				goto l_start;
			} else if (numTokens_AC_PRED_DIR - index_AC_PRED_DIR >= 1 && isSchedulable_skip()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_start;
					goto finished;
				}
				skip();
				i++;
				goto l_advance;
			} else if (numTokens_AC_PRED_DIR - index_AC_PRED_DIR >= 1 && numTokens_PTR - index_PTR >= 1 && isSchedulable_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_start;
					goto finished;
				}
				start();
				i++;
				goto l_pred;
			} else {
				_FSM_state = my_state_start;
				goto finished;
			}
	finished:
		
		read_end_PQF_AC();
		read_end_PTR();
		read_end_AC_PRED_DIR();
		
		write_end_QF_AC();
		
		
		return i;
	}
	
