	// Source file is "L/Research/src/fi/oulu/ee/mpeg4/part2/sp/Addressing_simple.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_texture_DCReconstruction_addressing_BTYPE->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_A;
	static unsigned int numFree_A;
	#define NUM_READERS_A 1
	#define SIZE_A 512
	#define tokens_A decoder_texture_DCReconstruction_addressing_A->contents
	static unsigned int index_B;
	static unsigned int numFree_B;
	#define NUM_READERS_B 1
	#define SIZE_B 512
	#define tokens_B decoder_texture_DCReconstruction_addressing_B->contents
	static unsigned int index_C;
	static unsigned int numFree_C;
	#define NUM_READERS_C 1
	#define SIZE_C 512
	#define tokens_C decoder_texture_DCReconstruction_addressing_C->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define ConstantsBtype_INTRA 1024
	static i8 mbx = 0;
	static i32 top_edge = 1;
	static i32 left_edge;
	static i8 comp = 0;
	static i8 mbwidth = 0;
	#define BUF_SIZE 14
	static i32 coded[112] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static i8 ptr;
	static i8 ptr_left;
	static i8 ptr_above;
	static i8 ptr_above_left;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_advance,
		my_state_geth,
		my_state_getw,
		my_state_predict,
		my_state_read
	};

	static char *stateNames[] = {
		"advance",
		"geth",
		"getw",
		"predict",
		"read"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_BTYPE() {
		index_BTYPE = decoder_texture_DCReconstruction_addressing_BTYPE->read_inds[2];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_addressing_BTYPE, 2);
	}
	
	static void read_end_BTYPE() {
		decoder_texture_DCReconstruction_addressing_BTYPE->read_inds[2] = index_BTYPE;
	}

	static void write_A() {
		index_A = (*decoder_texture_DCReconstruction_addressing_A->write_ind);
		numFree_A = index_A + fifo_i16_get_room(decoder_texture_DCReconstruction_addressing_A, NUM_READERS_A);
	}
	
	static void write_end_A() {
		(*decoder_texture_DCReconstruction_addressing_A->write_ind) = index_A;
	}
	static void write_B() {
		index_B = (*decoder_texture_DCReconstruction_addressing_B->write_ind);
		numFree_B = index_B + fifo_i16_get_room(decoder_texture_DCReconstruction_addressing_B, NUM_READERS_B);
	}
	
	static void write_end_B() {
		(*decoder_texture_DCReconstruction_addressing_B->write_ind) = index_B;
	}
	static void write_C() {
		index_C = (*decoder_texture_DCReconstruction_addressing_C->write_ind);
		numFree_C = index_C + fifo_i16_get_room(decoder_texture_DCReconstruction_addressing_C, NUM_READERS_C);
	}
	
	static void write_end_C() {
		(*decoder_texture_DCReconstruction_addressing_C->write_ind) = index_C;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i8 decrement(i8 p);
	static i16 access(i8 p, i8 c);

	static i8 decrement(i8 p) {
		i32 tmp_if;
		i32 local_BUF_SIZE;
	
		if (p == 1) {
			local_BUF_SIZE = BUF_SIZE;
			tmp_if = local_BUF_SIZE - 1;
		} else {
			tmp_if = p - 1;
		}
		return tmp_if;
	}
	static i16 access(i8 p, i8 c) {
	
		return p << 3 | c;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_start() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		result = (cmd & local_NEWVOP) != 0;
		return result;
	}
	
	static void start() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		mbx = 0;
		top_edge = 1;
		left_edge = 1;
		comp = 0;
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_getw() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void getw_replaced() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 w;
	
		w = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		mbwidth = w;
		ptr = 1;
		ptr_left = 2;
		ptr_above = 1 + w;
		ptr_above_left = 2 + w;
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_geth() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void geth() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 h;
	
		h = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_intra() {
		i32 result;
		i16 btype;
		i32 local_INTRA;
	
		btype = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTRA = ConstantsBtype_INTRA;
		result = (btype & local_INTRA) != 0;
		return result;
	}
	
	static void read_intra() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 btype;
		i8 local_ptr;
		i8 local_comp;
		i16 tmp_access;
	
		btype = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_ptr = ptr;
		local_comp = comp;
		tmp_access = access(local_ptr, local_comp);
		coded[tmp_access] = 1;
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_other() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void read_other() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 btype;
		i8 local_ptr;
		i8 local_comp;
		i16 tmp_access;
	
		btype = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_ptr = ptr;
		local_comp = comp;
		tmp_access = access(local_ptr, local_comp);
		coded[tmp_access] = 0;
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_advance() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void advance() {
	
		// Compute aligned port indexes
	
		i8 local_comp;
		i8 local_mbx;
		i8 local_mbwidth;
		i8 local_ptr;
		i8 tmp_decrement;
		i8 local_ptr_left;
		i8 tmp_decrement0;
		i8 local_ptr_above;
		i8 tmp_decrement1;
		i8 local_ptr_above_left;
		i8 tmp_decrement2;
	
		local_comp = comp;
		comp = local_comp + 1;
		local_comp = comp;
		if (local_comp == 6) {
			comp = 0;
			local_mbx = mbx;
			mbx = local_mbx + 1;
			left_edge = 0;
			local_mbx = mbx;
			local_mbwidth = mbwidth;
			if (local_mbx == local_mbwidth) {
				mbx = 0;
				top_edge = 0;
				left_edge = 1;
			}
			local_ptr = ptr;
			tmp_decrement = decrement(local_ptr);
			ptr = tmp_decrement;
			local_ptr_left = ptr_left;
			tmp_decrement0 = decrement(local_ptr_left);
			ptr_left = tmp_decrement0;
			local_ptr_above = ptr_above;
			tmp_decrement1 = decrement(local_ptr_above);
			ptr_above = tmp_decrement1;
			local_ptr_above_left = ptr_above_left;
			tmp_decrement2 = decrement(local_ptr_above_left);
			ptr_above_left = tmp_decrement2;
		}
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_predict() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void predict() {
	
		// Compute aligned port indexes
		i32 index_aligned_A = index_A % SIZE_A;
		i32 index_aligned_B = index_B % SIZE_B;
		i32 index_aligned_C = index_C % SIZE_C;
	
		i16 a;
		i16 b;
		i16 c;
		i8 local_comp;
		i32 local_left_edge;
		i8 local_ptr_left;
		i32 tmp_coded;
		i32 local_top_edge;
		i8 local_ptr_above_left;
		i32 tmp_coded0;
		i8 local_ptr_above;
		i32 tmp_coded1;
		i8 local_ptr;
		i32 tmp_coded2;
		i32 tmp_coded3;
		i32 tmp_coded4;
		i32 tmp_coded5;
		i32 tmp_coded6;
		i32 tmp_coded7;
		i32 tmp_coded8;
		i32 tmp_coded9;
		i32 tmp_coded10;
		i32 tmp_coded11;
		i32 tmp_coded12;
		i32 tmp_coded13;
	
		local_comp = comp;
		if (local_comp == 0) {
			a = 0;
			b = 0;
			c = 0;
			local_left_edge = left_edge;
			if (!local_left_edge) {
				local_ptr_left = ptr_left;
				a = access(local_ptr_left, 1);
				tmp_coded = coded[a];
				if (!tmp_coded) {
					a = 0;
				}
				local_top_edge = top_edge;
				if (!local_top_edge) {
					local_ptr_above_left = ptr_above_left;
					b = access(local_ptr_above_left, 3);
					tmp_coded0 = coded[b];
					if (!tmp_coded0) {
						b = 0;
					}
				}
			}
			local_top_edge = top_edge;
			if (!local_top_edge) {
				local_ptr_above = ptr_above;
				c = access(local_ptr_above, 2);
				tmp_coded1 = coded[c];
				if (!tmp_coded1) {
					c = 0;
				}
			}
		}
		local_comp = comp;
		if (local_comp == 1) {
			local_ptr = ptr;
			a = access(local_ptr, 0);
			b = 0;
			c = 0;
			tmp_coded2 = coded[a];
			if (!tmp_coded2) {
				a = 0;
			}
			local_top_edge = top_edge;
			if (!local_top_edge) {
				local_ptr_above = ptr_above;
				b = access(local_ptr_above, 2);
				tmp_coded3 = coded[b];
				if (!tmp_coded3) {
					b = 0;
				}
				local_ptr_above = ptr_above;
				c = access(local_ptr_above, 3);
				tmp_coded4 = coded[c];
				if (!tmp_coded4) {
					c = 0;
				}
			}
		}
		local_comp = comp;
		if (local_comp == 2) {
			a = 0;
			b = 0;
			local_ptr = ptr;
			c = access(local_ptr, 0);
			local_left_edge = left_edge;
			if (!local_left_edge) {
				local_ptr_left = ptr_left;
				a = access(local_ptr_left, 3);
				tmp_coded5 = coded[a];
				if (!tmp_coded5) {
					a = 0;
				}
				local_ptr_left = ptr_left;
				b = access(local_ptr_left, 1);
				tmp_coded6 = coded[b];
				if (!tmp_coded6) {
					b = 0;
				}
			}
			tmp_coded7 = coded[c];
			if (!tmp_coded7) {
				c = 0;
			}
		}
		local_comp = comp;
		if (local_comp == 3) {
			local_ptr = ptr;
			a = access(local_ptr, 2);
			local_ptr = ptr;
			b = access(local_ptr, 0);
			local_ptr = ptr;
			c = access(local_ptr, 1);
			tmp_coded8 = coded[a];
			if (!tmp_coded8) {
				a = 0;
			}
			tmp_coded9 = coded[b];
			if (!tmp_coded9) {
				b = 0;
			}
			tmp_coded10 = coded[c];
			if (!tmp_coded10) {
				c = 0;
			}
		}
		local_comp = comp;
		local_comp = comp;
		if (local_comp == 4 || local_comp == 5) {
			a = 0;
			b = 0;
			c = 0;
			local_left_edge = left_edge;
			if (!local_left_edge) {
				local_ptr_left = ptr_left;
				local_comp = comp;
				a = access(local_ptr_left, local_comp);
				tmp_coded11 = coded[a];
				if (!tmp_coded11) {
					a = 0;
				}
				local_top_edge = top_edge;
				if (!local_top_edge) {
					local_ptr_above_left = ptr_above_left;
					local_comp = comp;
					b = access(local_ptr_above_left, local_comp);
					tmp_coded12 = coded[b];
					if (!tmp_coded12) {
						b = 0;
					}
				}
			}
			local_top_edge = top_edge;
			if (!local_top_edge) {
				local_ptr_above = ptr_above;
				local_comp = comp;
				c = access(local_ptr_above, local_comp);
				tmp_coded13 = coded[c];
				if (!tmp_coded13) {
					c = 0;
				}
			}
		}
		tokens_A[(index_A + (0)) % SIZE_A] = a;
		tokens_B[(index_B + (0)) % SIZE_B] = b;
		tokens_C[(index_C + (0)) % SIZE_C] = c;
	
		// Update ports indexes
		index_A += 1;
		index_B += 1;
		index_C += 1;
		
		write_end_A();
		write_end_B();
		write_end_C();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_DCReconstruction_addressing_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_read;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_texture_DCReconstruction_addressing_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_DCReconstruction_addressing_initialize();
			needInit = 0;
		}
	
		read_BTYPE();
		write_A();
		write_B();
		write_C();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_advance:
			goto l_advance;
		case my_state_geth:
			goto l_geth;
		case my_state_getw:
			goto l_getw;
		case my_state_predict:
			goto l_predict;
		case my_state_read:
			goto l_read;
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
				goto l_read;
			} else {
				_FSM_state = my_state_advance;
				goto finished;
			}
		l_geth:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_geth()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_geth;
					goto finished;
				}
				geth();
				i++;
				goto l_read;
			} else {
				_FSM_state = my_state_geth;
				goto finished;
			}
		l_getw:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_getw()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_getw;
					goto finished;
				}
				getw_replaced();
				i++;
				goto l_geth;
			} else {
				_FSM_state = my_state_getw;
				goto finished;
			}
		l_predict:
			if (isSchedulable_predict()) {
				int stop = 0;
				if (1 > SIZE_A - index_A + decoder_texture_DCReconstruction_addressing_A->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_B - index_B + decoder_texture_DCReconstruction_addressing_B->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_C - index_C + decoder_texture_DCReconstruction_addressing_C->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_predict;
					goto finished;
				}
				predict();
				i++;
				goto l_advance;
			} else {
				_FSM_state = my_state_predict;
				goto finished;
			}
		l_read:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				start();
				i++;
				goto l_getw;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_intra()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_intra();
				i++;
				goto l_predict;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_other()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_other();
				i++;
				goto l_advance;
			} else {
				_FSM_state = my_state_read;
				goto finished;
			}
	finished:
		
		read_end_BTYPE();
		
		write_end_A();
		write_end_B();
		write_end_C();
		
		
		return i;
	}
	
