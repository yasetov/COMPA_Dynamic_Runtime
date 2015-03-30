	// Source file is "L/Research/src/fi/oulu/ee/mpeg4/part2/sp/Algo_DCRinvpred.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_texture_DCReconstruction_invpred_BTYPE->contents
	
	
	static unsigned int index_A;
	static unsigned int numTokens_A;
	#define SIZE_A 512
	#define tokens_A 	decoder_texture_DCReconstruction_invpred_A->contents
	
	
	static unsigned int index_B;
	static unsigned int numTokens_B;
	#define SIZE_B 512
	#define tokens_B 	decoder_texture_DCReconstruction_invpred_B->contents
	
	
	static unsigned int index_C;
	static unsigned int numTokens_C;
	#define SIZE_C 512
	#define tokens_C 	decoder_texture_DCReconstruction_invpred_C->contents
	
	
	static unsigned int index_QFS_DC;
	static unsigned int numTokens_QFS_DC;
	#define SIZE_QFS_DC 512
	#define tokens_QFS_DC 	decoder_texture_DCReconstruction_invpred_QFS_DC->contents
	
	
	static unsigned int index_QP;
	static unsigned int numTokens_QP;
	#define SIZE_QP 512
	#define tokens_QP 	decoder_texture_DCReconstruction_invpred_QP->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_QF_DC;
	static unsigned int numFree_QF_DC;
	#define NUM_READERS_QF_DC 1
	#define SIZE_QF_DC 512
	#define tokens_QF_DC decoder_texture_DCReconstruction_invpred_QF_DC->contents
	static unsigned int index_PTR;
	static unsigned int numFree_PTR;
	#define NUM_READERS_PTR 1
	#define SIZE_PTR 512
	#define tokens_PTR decoder_texture_DCReconstruction_invpred_PTR->contents
	static unsigned int index_AC_PRED_DIR;
	static unsigned int numFree_AC_PRED_DIR;
	#define NUM_READERS_AC_PRED_DIR 1
	#define SIZE_AC_PRED_DIR 512
	#define tokens_AC_PRED_DIR decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->contents
	static unsigned int index_MODE;
	static unsigned int numFree_MODE;
	#define NUM_READERS_MODE 1
	#define SIZE_MODE 512
	#define tokens_MODE decoder_texture_DCReconstruction_invpred_MODE->contents
	static unsigned int index_SIGNED;
	static unsigned int numFree_SIGNED;
	#define NUM_READERS_SIGNED 1
	#define SIZE_SIGNED 512
	#define tokens_SIGNED decoder_texture_DCReconstruction_invpred_SIGNED->contents
	static unsigned int index_QUANT;
	static unsigned int numFree_QUANT;
	#define NUM_READERS_QUANT 1
	#define SIZE_QUANT 512
	#define tokens_QUANT decoder_texture_DCReconstruction_invpred_QUANT->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define ConstantsBtype_INTER 512
	#define Constants_ACCODED 2
	#define ConstantsBtype_INTRA 1024
	#define Constants_ACPRED 1
	static i8 s_QP;
	static i8 round;
	#define BUF_SIZE 112
	static i32 ptr = 8;
	static i8 comp = 0;
	static i16 dc_buf[112] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
	static i16 dc_pred;
	static i8 scaler;
	static i16 dc_val;
	static i32 is_signed;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_advance,
		my_state_geth,
		my_state_getw,
		my_state_inter,
		my_state_intra,
		my_state_read,
		my_state_sat
	};

	static char *stateNames[] = {
		"advance",
		"geth",
		"getw",
		"inter",
		"intra",
		"read",
		"sat"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_BTYPE() {
		index_BTYPE = decoder_texture_DCReconstruction_invpred_BTYPE->read_inds[3];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_invpred_BTYPE, 3);
	}
	
	static void read_end_BTYPE() {
		decoder_texture_DCReconstruction_invpred_BTYPE->read_inds[3] = index_BTYPE;
	}
	static void read_A() {
		index_A = decoder_texture_DCReconstruction_invpred_A->read_inds[0];
		numTokens_A = index_A + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_invpred_A, 0);
	}
	
	static void read_end_A() {
		decoder_texture_DCReconstruction_invpred_A->read_inds[0] = index_A;
	}
	static void read_B() {
		index_B = decoder_texture_DCReconstruction_invpred_B->read_inds[0];
		numTokens_B = index_B + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_invpred_B, 0);
	}
	
	static void read_end_B() {
		decoder_texture_DCReconstruction_invpred_B->read_inds[0] = index_B;
	}
	static void read_C() {
		index_C = decoder_texture_DCReconstruction_invpred_C->read_inds[0];
		numTokens_C = index_C + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_invpred_C, 0);
	}
	
	static void read_end_C() {
		decoder_texture_DCReconstruction_invpred_C->read_inds[0] = index_C;
	}
	static void read_QFS_DC() {
		index_QFS_DC = decoder_texture_DCReconstruction_invpred_QFS_DC->read_inds[0];
		numTokens_QFS_DC = index_QFS_DC + fifo_i16_get_num_tokens(decoder_texture_DCReconstruction_invpred_QFS_DC, 0);
	}
	
	static void read_end_QFS_DC() {
		decoder_texture_DCReconstruction_invpred_QFS_DC->read_inds[0] = index_QFS_DC;
	}
	static void read_QP() {
		index_QP = decoder_texture_DCReconstruction_invpred_QP->read_inds[0];
		numTokens_QP = index_QP + fifo_i8_get_num_tokens(decoder_texture_DCReconstruction_invpred_QP, 0);
	}
	
	static void read_end_QP() {
		decoder_texture_DCReconstruction_invpred_QP->read_inds[0] = index_QP;
	}

	static void write_QF_DC() {
		index_QF_DC = (*decoder_texture_DCReconstruction_invpred_QF_DC->write_ind);
		numFree_QF_DC = index_QF_DC + fifo_i16_get_room(decoder_texture_DCReconstruction_invpred_QF_DC, NUM_READERS_QF_DC);
	}
	
	static void write_end_QF_DC() {
		(*decoder_texture_DCReconstruction_invpred_QF_DC->write_ind) = index_QF_DC;
	}
	static void write_PTR() {
		index_PTR = (*decoder_texture_DCReconstruction_invpred_PTR->write_ind);
		numFree_PTR = index_PTR + fifo_i16_get_room(decoder_texture_DCReconstruction_invpred_PTR, NUM_READERS_PTR);
	}
	
	static void write_end_PTR() {
		(*decoder_texture_DCReconstruction_invpred_PTR->write_ind) = index_PTR;
	}
	static void write_AC_PRED_DIR() {
		index_AC_PRED_DIR = (*decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->write_ind);
		numFree_AC_PRED_DIR = index_AC_PRED_DIR + fifo_i8_get_room(decoder_texture_DCReconstruction_invpred_AC_PRED_DIR, NUM_READERS_AC_PRED_DIR);
	}
	
	static void write_end_AC_PRED_DIR() {
		(*decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->write_ind) = index_AC_PRED_DIR;
	}
	static void write_MODE() {
		index_MODE = (*decoder_texture_DCReconstruction_invpred_MODE->write_ind);
		numFree_MODE = index_MODE + fifo_i8_get_room(decoder_texture_DCReconstruction_invpred_MODE, NUM_READERS_MODE);
	}
	
	static void write_end_MODE() {
		(*decoder_texture_DCReconstruction_invpred_MODE->write_ind) = index_MODE;
	}
	static void write_SIGNED() {
		index_SIGNED = (*decoder_texture_DCReconstruction_invpred_SIGNED->write_ind);
		numFree_SIGNED = index_SIGNED + fifo_i32_get_room(decoder_texture_DCReconstruction_invpred_SIGNED, NUM_READERS_SIGNED);
	}
	
	static void write_end_SIGNED() {
		(*decoder_texture_DCReconstruction_invpred_SIGNED->write_ind) = index_SIGNED;
	}
	static void write_QUANT() {
		index_QUANT = (*decoder_texture_DCReconstruction_invpred_QUANT->write_ind);
		numFree_QUANT = index_QUANT + fifo_i8_get_room(decoder_texture_DCReconstruction_invpred_QUANT, NUM_READERS_QUANT);
	}
	
	static void write_end_QUANT() {
		(*decoder_texture_DCReconstruction_invpred_QUANT->write_ind) = index_QUANT;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i32 Math_abs(i32 x);
	static i32 saturate(i32 x);
	static i8 dc_scaler();

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
	static i8 dc_scaler() {
		i8 local_comp;
		i8 tmp_if;
		i8 local_s_QP;
	
		local_comp = comp;
		if ((local_comp & 4) == 0) {
			local_s_QP = s_QP;
			local_s_QP = s_QP;
			if (local_s_QP > 0 && local_s_QP < 5) {
				tmp_if = 8;
			} else {
				local_s_QP = s_QP;
				local_s_QP = s_QP;
				if (local_s_QP > 4 && local_s_QP < 9) {
					local_s_QP = s_QP;
					tmp_if = 2 * local_s_QP;
				} else {
					local_s_QP = s_QP;
					local_s_QP = s_QP;
					if (local_s_QP > 8 && local_s_QP < 25) {
						local_s_QP = s_QP;
						tmp_if = local_s_QP + 8;
					} else {
						local_s_QP = s_QP;
						tmp_if = 2 * local_s_QP - 16;
					}
				}
			}
		} else {
			local_s_QP = s_QP;
			local_s_QP = s_QP;
			if (local_s_QP > 0 && local_s_QP < 5) {
				tmp_if = 8;
			} else {
				local_s_QP = s_QP;
				local_s_QP = s_QP;
				if (local_s_QP > 4 && local_s_QP < 25) {
					local_s_QP = s_QP;
					tmp_if = (local_s_QP + 13) >> 1;
				} else {
					local_s_QP = s_QP;
					tmp_if = local_s_QP - 6;
				}
			}
		}
		return tmp_if;
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
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		comp = 0;
		ptr = 8;
		tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR] = -2;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_AC_PRED_DIR += 1;
		
		read_end_BTYPE();
		write_end_AC_PRED_DIR();
	}
	static i32 isSchedulable_skip() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void skip() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 x;
	
		x = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_inter_ac() {
		i32 result;
		i16 cmd;
		i32 local_INTER;
		i32 local_ACCODED;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTER = ConstantsBtype_INTER;
		local_ACCODED = Constants_ACCODED;
		result = (cmd & local_INTER) != 0 && (cmd & local_ACCODED) != 0;
		return result;
	}
	
	static void read_inter_ac() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_QP = index_QP % SIZE_QP;
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
		i32 index_aligned_MODE = index_MODE % SIZE_MODE;
		i32 index_aligned_PTR = index_PTR % SIZE_PTR;
		i32 index_aligned_SIGNED = index_SIGNED % SIZE_SIGNED;
		i32 index_aligned_QUANT = index_QUANT % SIZE_QUANT;
	
		i16 cmd;
		i8 qp;
		i8 local_s_QP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		qp = tokens_QP[(index_QP + (0)) % SIZE_QP];
		is_signed = 1;
		s_QP = qp;
		local_s_QP = s_QP;
		round = local_s_QP & 1 ^ 1;
		tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR] = 0;
		tokens_MODE[(index_MODE + (0)) % SIZE_MODE] = 0;
		tokens_PTR[(index_PTR + (0)) % SIZE_PTR] = 0;
		tokens_SIGNED[(index_SIGNED + (0)) % SIZE_SIGNED] = 1;
		local_s_QP = s_QP;
		tokens_QUANT[(index_QUANT + (0)) % SIZE_QUANT] = local_s_QP;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_QP += 1;
		index_AC_PRED_DIR += 1;
		index_MODE += 1;
		index_PTR += 1;
		index_SIGNED += 1;
		index_QUANT += 1;
		
		read_end_BTYPE();
		read_end_QP();
		write_end_AC_PRED_DIR();
		write_end_MODE();
		write_end_PTR();
		write_end_SIGNED();
		write_end_QUANT();
	}
	static i32 isSchedulable_read_other() {
		i32 result;
		i16 cmd;
		i32 local_INTRA;
		i32 local_ACCODED;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTRA = ConstantsBtype_INTRA;
		local_ACCODED = Constants_ACCODED;
		result = (cmd & local_INTRA) == 0 && (cmd & local_ACCODED) == 0;
		return result;
	}
	
	static void read_other() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_QP = index_QP % SIZE_QP;
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
	
		i16 cmd;
		i8 qp;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		qp = tokens_QP[(index_QP + (0)) % SIZE_QP];
		s_QP = qp;
		tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR] = -1;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_QP += 1;
		index_AC_PRED_DIR += 1;
		
		read_end_BTYPE();
		read_end_QP();
		write_end_AC_PRED_DIR();
	}
	static i32 isSchedulable_read_intra() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void read_intra() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_QP = index_QP % SIZE_QP;
		i32 index_aligned_A = index_A % SIZE_A;
		i32 index_aligned_B = index_B % SIZE_B;
		i32 index_aligned_C = index_C % SIZE_C;
		i32 index_aligned_PTR = index_PTR % SIZE_PTR;
		i32 index_aligned_AC_PRED_DIR = index_AC_PRED_DIR % SIZE_AC_PRED_DIR;
		i32 index_aligned_MODE = index_MODE % SIZE_MODE;
		i32 index_aligned_SIGNED = index_SIGNED % SIZE_SIGNED;
		i32 index_aligned_QUANT = index_QUANT % SIZE_QUANT;
	
		i16 cmd;
		i8 qp;
		i16 a;
		i16 b;
		i16 c;
		i16 dca;
		i16 dcb;
		i16 dcc;
		i16 horiz;
		i16 vert;
		i32 top;
		i8 s;
		i32 local_ACPRED;
		i32 ac;
		i8 tmp_dc_scaler;
		i16 tmp_if;
		i8 local_scaler;
		i32 local_is_signed;
		i8 local_s_QP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		qp = tokens_QP[(index_QP + (0)) % SIZE_QP];
		a = tokens_A[(index_A + (0)) % SIZE_A];
		b = tokens_B[(index_B + (0)) % SIZE_B];
		c = tokens_C[(index_C + (0)) % SIZE_C];
		dca = dc_buf[a];
		dcb = dc_buf[b];
		dcc = dc_buf[c];
		horiz = Math_abs(dcb - dcc);
		vert = Math_abs(dca - dcb);
		top = vert < horiz;
		local_ACPRED = Constants_ACPRED;
		ac = (cmd & local_ACPRED) != 0;
		s_QP = qp;
		tmp_dc_scaler = dc_scaler();
		scaler = tmp_dc_scaler;
		if (top) {
			tmp_if = dcc;
		} else {
			tmp_if = dca;
		}
		local_scaler = scaler;
		local_scaler = scaler;
		dc_pred = (tmp_if + (local_scaler >> 1)) / local_scaler;
		if (!ac) {
			s = 0;
		} else {
			if (top) {
				s = 2;
			} else {
				s = 1;
			}
		}
		local_scaler = scaler;
		is_signed = local_scaler == 0;
		if (top) {
			tokens_PTR[(index_PTR + (0)) % SIZE_PTR] = c;
		} else {
			tokens_PTR[(index_PTR + (0)) % SIZE_PTR] = a;
		}
		tokens_AC_PRED_DIR[(index_AC_PRED_DIR + (0)) % SIZE_AC_PRED_DIR] = s;
		tokens_MODE[(index_MODE + (0)) % SIZE_MODE] = s;
		local_is_signed = is_signed;
		tokens_SIGNED[(index_SIGNED + (0)) % SIZE_SIGNED] = local_is_signed;
		local_s_QP = s_QP;
		tokens_QUANT[(index_QUANT + (0)) % SIZE_QUANT] = local_s_QP;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_QP += 1;
		index_A += 1;
		index_B += 1;
		index_C += 1;
		index_PTR += 1;
		index_AC_PRED_DIR += 1;
		index_MODE += 1;
		index_SIGNED += 1;
		index_QUANT += 1;
		
		read_end_BTYPE();
		read_end_QP();
		read_end_A();
		read_end_B();
		read_end_C();
		write_end_PTR();
		write_end_AC_PRED_DIR();
		write_end_MODE();
		write_end_SIGNED();
		write_end_QUANT();
	}
	static i32 isSchedulable_getdc_inter() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void getdc_inter() {
	
		// Compute aligned port indexes
		i32 index_aligned_QFS_DC = index_QFS_DC % SIZE_QFS_DC;
	
		i16 dc;
		i8 local_s_QP;
		i32 tmp_abs;
		i8 local_round;
		i16 v;
		i32 local_is_signed;
	
		dc = tokens_QFS_DC[(index_QFS_DC + (0)) % SIZE_QFS_DC];
		local_s_QP = s_QP;
		tmp_abs = Math_abs(dc);
		local_round = round;
		v = local_s_QP * ((tmp_abs << 1) + 1) - local_round;
		if (dc == 0) {
			dc_val = 0;
		} else {
			local_is_signed = is_signed;
			if (!local_is_signed) {
				dc_val = dc;
			} else {
				if (dc < 0) {
					dc_val = -v;
				} else {
					dc_val = v;
				}
			}
		}
	
		// Update ports indexes
		index_QFS_DC += 1;
		
		read_end_QFS_DC();
	}
	static i32 isSchedulable_getdc_intra() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void getdc_intra() {
	
		// Compute aligned port indexes
		i32 index_aligned_QFS_DC = index_QFS_DC % SIZE_QFS_DC;
	
		i16 dc;
		i16 local_dc_pred;
		i8 local_scaler;
	
		dc = tokens_QFS_DC[(index_QFS_DC + (0)) % SIZE_QFS_DC];
		local_dc_pred = dc_pred;
		local_scaler = scaler;
		dc_val = (dc + local_dc_pred) * local_scaler;
	
		// Update ports indexes
		index_QFS_DC += 1;
		
		read_end_QFS_DC();
	}
	static i32 isSchedulable_sat() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void sat() {
	
		// Compute aligned port indexes
		i32 index_aligned_QF_DC = index_QF_DC % SIZE_QF_DC;
	
		i16 local_dc_val;
		i16 dc;
		i32 local_ptr;
		i8 local_comp;
	
		local_dc_val = dc_val;
		dc = saturate(local_dc_val);
		local_ptr = ptr;
		local_comp = comp;
		dc_buf[local_ptr | local_comp] = dc;
		tokens_QF_DC[(index_QF_DC + (0)) % SIZE_QF_DC] = dc;
	
		// Update ports indexes
		index_QF_DC += 1;
		
		write_end_QF_DC();
	}
	static i32 isSchedulable_advance() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void advance() {
	
		// Compute aligned port indexes
	
		i8 local_comp;
		i32 local_ptr;
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

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_DCReconstruction_invpred_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_read;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_texture_DCReconstruction_invpred_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_DCReconstruction_invpred_initialize();
			needInit = 0;
		}
	
		read_BTYPE();
		read_A();
		read_B();
		read_C();
		read_QFS_DC();
		read_QP();
		write_QF_DC();
		write_PTR();
		write_AC_PRED_DIR();
		write_MODE();
		write_SIGNED();
		write_QUANT();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_advance:
			goto l_advance;
		case my_state_geth:
			goto l_geth;
		case my_state_getw:
			goto l_getw;
		case my_state_inter:
			goto l_inter;
		case my_state_intra:
			goto l_intra;
		case my_state_read:
			goto l_read;
		case my_state_sat:
			goto l_sat;
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
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_skip()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_geth;
					goto finished;
				}
				skip();
				i++;
				goto l_read;
			} else {
				_FSM_state = my_state_geth;
				goto finished;
			}
		l_getw:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_skip()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_getw;
					goto finished;
				}
				skip();
				i++;
				goto l_geth;
			} else {
				_FSM_state = my_state_getw;
				goto finished;
			}
		l_inter:
			if (numTokens_QFS_DC - index_QFS_DC >= 1 && isSchedulable_getdc_inter()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_inter;
					goto finished;
				}
				getdc_inter();
				i++;
				goto l_sat;
			} else {
				_FSM_state = my_state_inter;
				goto finished;
			}
		l_intra:
			if (numTokens_QFS_DC - index_QFS_DC >= 1 && isSchedulable_getdc_intra()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_intra;
					goto finished;
				}
				getdc_intra();
				i++;
				goto l_sat;
			} else {
				_FSM_state = my_state_intra;
				goto finished;
			}
		l_read:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_start()) {
				int stop = 0;
				if (1 > SIZE_AC_PRED_DIR - index_AC_PRED_DIR + decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				start();
				i++;
				goto l_getw;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && numTokens_QP - index_QP >= 1 && isSchedulable_read_inter_ac()) {
				int stop = 0;
				if (1 > SIZE_AC_PRED_DIR - index_AC_PRED_DIR + decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_MODE - index_MODE + decoder_texture_DCReconstruction_invpred_MODE->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_PTR - index_PTR + decoder_texture_DCReconstruction_invpred_PTR->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_SIGNED - index_SIGNED + decoder_texture_DCReconstruction_invpred_SIGNED->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_QUANT - index_QUANT + decoder_texture_DCReconstruction_invpred_QUANT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_inter_ac();
				i++;
				goto l_inter;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && numTokens_QP - index_QP >= 1 && isSchedulable_read_other()) {
				int stop = 0;
				if (1 > SIZE_AC_PRED_DIR - index_AC_PRED_DIR + decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_other();
				i++;
				goto l_advance;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && numTokens_QP - index_QP >= 1 && numTokens_A - index_A >= 1 && numTokens_B - index_B >= 1 && numTokens_C - index_C >= 1 && isSchedulable_read_intra()) {
				int stop = 0;
				if (1 > SIZE_PTR - index_PTR + decoder_texture_DCReconstruction_invpred_PTR->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_AC_PRED_DIR - index_AC_PRED_DIR + decoder_texture_DCReconstruction_invpred_AC_PRED_DIR->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_MODE - index_MODE + decoder_texture_DCReconstruction_invpred_MODE->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_SIGNED - index_SIGNED + decoder_texture_DCReconstruction_invpred_SIGNED->read_inds[0]) {
					stop = 1;
				}
				if (1 > SIZE_QUANT - index_QUANT + decoder_texture_DCReconstruction_invpred_QUANT->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_intra();
				i++;
				goto l_intra;
			} else {
				_FSM_state = my_state_read;
				goto finished;
			}
		l_sat:
			if (isSchedulable_sat()) {
				int stop = 0;
				if (1 > SIZE_QF_DC - index_QF_DC + decoder_texture_DCReconstruction_invpred_QF_DC->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_sat;
					goto finished;
				}
				sat();
				i++;
				goto l_advance;
			} else {
				_FSM_state = my_state_sat;
				goto finished;
			}
	finished:
		
		read_end_BTYPE();
		read_end_A();
		read_end_B();
		read_end_C();
		read_end_QFS_DC();
		read_end_QP();
		
		write_end_QF_DC();
		write_end_PTR();
		write_end_AC_PRED_DIR();
		write_end_MODE();
		write_end_SIGNED();
		write_end_QUANT();
		
		
		return i;
	}
	
