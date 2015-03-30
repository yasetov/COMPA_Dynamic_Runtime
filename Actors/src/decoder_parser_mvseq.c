	// Source file is "L/Research/src/org/ietr/mpeg4/part2/sp/parser/Mgnt_MVSequence.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_parser_mvseq_BTYPE->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_A;
	static unsigned int numFree_A;
	#define NUM_READERS_A 1
	#define SIZE_A 512
	#define tokens_A decoder_parser_mvseq_A->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define ConstantsBtype_INTER 512
	#define ConstantsBtype_MOTION 8
	#define Constants_FOURMV 4
	static i8 mbx = 0;
	static i32 top_edge = 1;
	static i32 right_edge = 0;
	static i8 comp = 0;
	static i8 mbwidth = 0;
	#define BUF_SIZE 16
	static i8 ptr;
	static i8 ptr_left;
	static i8 ptr_above;
	static i8 ptr_above_right;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_geth,
		my_state_getw,
		my_state_read
	};

	static char *stateNames[] = {
		"geth",
		"getw",
		"read"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_BTYPE() {
		index_BTYPE = decoder_parser_mvseq_BTYPE->read_inds[0];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_parser_mvseq_BTYPE, 0);
	}
	
	static void read_end_BTYPE() {
		decoder_parser_mvseq_BTYPE->read_inds[0] = index_BTYPE;
	}

	static void write_A() {
		index_A = (*decoder_parser_mvseq_A->write_ind);
		numFree_A = index_A + fifo_i16_get_room(decoder_parser_mvseq_A, NUM_READERS_A);
	}
	
	static void write_end_A() {
		(*decoder_parser_mvseq_A->write_ind) = index_A;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i8 decrement(i8 p);
	static i16 access(i8 mbptr, i8 c);

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
	static i16 access(i8 mbptr, i8 c) {
	
		return mbptr << 3 | c & 3;
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
		right_edge = 0;
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
		ptr_above = w + 1;
		ptr_above_right = w;
	
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
	static i32 isSchedulable_read_noPredict() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
		i8 local_comp;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_FOURMV;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		local_comp = comp;
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_comp = comp;
		local_FOURMV = Constants_FOURMV;
		result = (cmd & local_NEWVOP) == 0 && (local_comp > 3 || (cmd & local_INTER) == 0 || (cmd & local_MOTION) == 0 || local_comp != 0 && (cmd & local_FOURMV) == 0);
		return result;
	}
	
	static void read_noPredict() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
		i8 local_comp;
		i8 local_mbx;
		i8 local_ptr;
		i8 tmp_decrement;
		i8 local_ptr_left;
		i8 tmp_decrement0;
		i8 local_ptr_above;
		i8 tmp_decrement1;
		i8 local_ptr_above_right;
		i8 tmp_decrement2;
		i32 local_right_edge;
		i8 local_mbwidth;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_comp = comp;
		comp = local_comp + 1;
		local_comp = comp;
		if (local_comp == 6) {
			comp = 0;
			local_mbx = mbx;
			mbx = local_mbx + 1;
			local_ptr = ptr;
			tmp_decrement = decrement(local_ptr);
			ptr = tmp_decrement;
			local_ptr_left = ptr_left;
			tmp_decrement0 = decrement(local_ptr_left);
			ptr_left = tmp_decrement0;
			local_ptr_above = ptr_above;
			tmp_decrement1 = decrement(local_ptr_above);
			ptr_above = tmp_decrement1;
			local_ptr_above_right = ptr_above_right;
			tmp_decrement2 = decrement(local_ptr_above_right);
			ptr_above_right = tmp_decrement2;
			local_right_edge = right_edge;
			if (local_right_edge) {
				mbx = 0;
				right_edge = 0;
				top_edge = 0;
			} else {
				local_mbx = mbx;
				local_mbwidth = mbwidth;
				if (local_mbx == local_mbwidth - 1) {
					right_edge = 1;
				}
			}
		}
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_predict_y0() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
		i32 local_INTER;
		i32 local_MOTION;
		i8 local_comp;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_comp = comp;
		result = (cmd & local_NEWVOP) == 0 && (cmd & local_INTER) != 0 && (cmd & local_MOTION) != 0 && local_comp == 0;
		return result;
	}
	
	static void read_predict_y0() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_A = index_A % SIZE_A;
	
		i16 cmd;
		i8 local_mbx;
		i8 local_ptr_left;
		i8 pl;
		i32 local_top_edge;
		i8 local_ptr_above;
		i8 pa;
		i32 local_right_edge;
		i8 local_ptr_above_right;
		i8 par;
		i8 local_comp;
		i16 tmp_access;
		i16 tmp_access0;
		i16 tmp_access1;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_mbx = mbx;
		if (local_mbx == 0) {
			pl = 0;
		} else {
			local_ptr_left = ptr_left;
			pl = local_ptr_left;
		}
		local_top_edge = top_edge;
		if (local_top_edge) {
			pa = 0;
		} else {
			local_ptr_above = ptr_above;
			pa = local_ptr_above;
		}
		local_top_edge = top_edge;
		local_right_edge = right_edge;
		if (local_top_edge || local_right_edge) {
			par = 0;
		} else {
			local_ptr_above_right = ptr_above_right;
			par = local_ptr_above_right;
		}
		local_comp = comp;
		comp = local_comp + 1;
		tmp_access = access(pl, 1);
		tokens_A[(index_A + (0)) % SIZE_A] = tmp_access;
		tmp_access0 = access(pa, 2);
		tokens_A[(index_A + (1)) % SIZE_A] = tmp_access0;
		tmp_access1 = access(par, 2);
		tokens_A[(index_A + (2)) % SIZE_A] = tmp_access1;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_A += 3;
		
		read_end_BTYPE();
		write_end_A();
	}
	static i32 isSchedulable_read_predict_y1() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_FOURMV;
		i8 local_comp;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_FOURMV = Constants_FOURMV;
		local_comp = comp;
		result = (cmd & local_NEWVOP) == 0 && (cmd & local_INTER) != 0 && (cmd & local_MOTION) != 0 && (cmd & local_FOURMV) != 0 && local_comp == 1;
		return result;
	}
	
	static void read_predict_y1() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_A = index_A % SIZE_A;
	
		i16 cmd;
		i32 local_top_edge;
		i8 local_ptr_above;
		i8 pa;
		i32 local_right_edge;
		i8 local_ptr_above_right;
		i8 par;
		i8 local_comp;
		i8 local_ptr;
		i16 tmp_access;
		i16 tmp_access0;
		i16 tmp_access1;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_top_edge = top_edge;
		if (local_top_edge) {
			pa = 0;
		} else {
			local_ptr_above = ptr_above;
			pa = local_ptr_above;
		}
		local_top_edge = top_edge;
		local_right_edge = right_edge;
		if (local_top_edge || local_right_edge) {
			par = 0;
		} else {
			local_ptr_above_right = ptr_above_right;
			par = local_ptr_above_right;
		}
		local_comp = comp;
		comp = local_comp + 1;
		local_ptr = ptr;
		tmp_access = access(local_ptr, 0);
		tokens_A[(index_A + (0)) % SIZE_A] = tmp_access;
		tmp_access0 = access(pa, 3);
		tokens_A[(index_A + (1)) % SIZE_A] = tmp_access0;
		tmp_access1 = access(par, 2);
		tokens_A[(index_A + (2)) % SIZE_A] = tmp_access1;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_A += 3;
		
		read_end_BTYPE();
		write_end_A();
	}
	static i32 isSchedulable_read_predict_y2() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_FOURMV;
		i8 local_comp;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_FOURMV = Constants_FOURMV;
		local_comp = comp;
		result = (cmd & local_NEWVOP) == 0 && (cmd & local_INTER) != 0 && (cmd & local_MOTION) != 0 && (cmd & local_FOURMV) != 0 && local_comp == 2;
		return result;
	}
	
	static void read_predict_y2() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_A = index_A % SIZE_A;
	
		i16 cmd;
		i8 local_mbx;
		i8 local_ptr_left;
		i8 pl;
		i8 local_comp;
		i16 tmp_access;
		i8 local_ptr;
		i16 tmp_access0;
		i16 tmp_access1;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_mbx = mbx;
		if (local_mbx == 0) {
			pl = 0;
		} else {
			local_ptr_left = ptr_left;
			pl = local_ptr_left;
		}
		local_comp = comp;
		comp = local_comp + 1;
		tmp_access = access(pl, 3);
		tokens_A[(index_A + (0)) % SIZE_A] = tmp_access;
		local_ptr = ptr;
		tmp_access0 = access(local_ptr, 0);
		tokens_A[(index_A + (1)) % SIZE_A] = tmp_access0;
		local_ptr = ptr;
		tmp_access1 = access(local_ptr, 1);
		tokens_A[(index_A + (2)) % SIZE_A] = tmp_access1;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_A += 3;
		
		read_end_BTYPE();
		write_end_A();
	}
	static i32 isSchedulable_read_predict_y3() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_FOURMV;
		i8 local_comp;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_FOURMV = Constants_FOURMV;
		local_comp = comp;
		result = (cmd & local_NEWVOP) == 0 && (cmd & local_INTER) != 0 && (cmd & local_MOTION) != 0 && (cmd & local_FOURMV) != 0 && local_comp == 3;
		return result;
	}
	
	static void read_predict_y3() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_A = index_A % SIZE_A;
	
		i16 cmd;
		i8 local_comp;
		i8 local_ptr;
		i16 tmp_access;
		i16 tmp_access0;
		i16 tmp_access1;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_comp = comp;
		comp = local_comp + 1;
		local_ptr = ptr;
		tmp_access = access(local_ptr, 2);
		tokens_A[(index_A + (0)) % SIZE_A] = tmp_access;
		local_ptr = ptr;
		tmp_access0 = access(local_ptr, 0);
		tokens_A[(index_A + (1)) % SIZE_A] = tmp_access0;
		local_ptr = ptr;
		tmp_access1 = access(local_ptr, 1);
		tokens_A[(index_A + (2)) % SIZE_A] = tmp_access1;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_A += 3;
		
		read_end_BTYPE();
		write_end_A();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_parser_mvseq_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_read;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_parser_mvseq_scheduler() {
		int i = 0;
		if(needInit){
			decoder_parser_mvseq_initialize();
			needInit = 0;
		}
	
		read_BTYPE();
		write_A();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_geth:
			goto l_geth;
		case my_state_getw:
			goto l_getw;
		case my_state_read:
			goto l_read;
		default:
			exit(1);
		}
	
		// FSM transitions
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
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_noPredict()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_noPredict();
				i++;
				goto l_read;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_predict_y0()) {
				int stop = 0;
				if (3 > SIZE_A - index_A + decoder_parser_mvseq_A->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_predict_y0();
				i++;
				goto l_read;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_predict_y1()) {
				int stop = 0;
				if (3 > SIZE_A - index_A + decoder_parser_mvseq_A->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_predict_y1();
				i++;
				goto l_read;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_predict_y2()) {
				int stop = 0;
				if (3 > SIZE_A - index_A + decoder_parser_mvseq_A->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_predict_y2();
				i++;
				goto l_read;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_predict_y3()) {
				int stop = 0;
				if (3 > SIZE_A - index_A + decoder_parser_mvseq_A->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_predict_y3();
				i++;
				goto l_read;
			} else {
				_FSM_state = my_state_read;
				goto finished;
			}
	finished:
		
		read_end_BTYPE();
		
		write_end_A();
		
		
		return i;
	}
	
