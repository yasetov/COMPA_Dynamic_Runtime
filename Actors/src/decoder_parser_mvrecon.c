	// Source file is "L/Research/src/org/ietr/mpeg4/part2/sp/parser/Mgnt_MVReconstruct.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_parser_mvrecon_BTYPE->contents
	
	
	static unsigned int index_MVIN;
	static unsigned int numTokens_MVIN;
	#define SIZE_MVIN 512
	#define tokens_MVIN 	decoder_parser_mvrecon_MVIN->contents
	
	
	static unsigned int index_A;
	static unsigned int numTokens_A;
	#define SIZE_A 512
	#define tokens_A 	decoder_parser_mvrecon_A->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_MV;
	static unsigned int numFree_MV;
	#define NUM_READERS_MV 1
	#define SIZE_MV 512
	#define tokens_MV decoder_parser_mvrecon_MV->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define ConstantsBtype_FCODE_MASK 448
	#define Constants_FCODE_SHIFT 6
	#define ConstantsBtype_INTER 512
	#define ConstantsBtype_MOTION 8
	#define Constants_FOURMV 4
	static i8 mbx = 0;
	static i32 top_edge = 1;
	static i8 comp = 0;
	static i8 mbwidth = 0;
	#define BUF_SIZE 128
	#define Y_SELECT 4
	static i16 ptr;
	static i16 buf[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static i16 mv_rsize;
	static i16 mv_range;
	static i16 mv_low;
	static i16 mv_high;
	static i32 fourmv;
	static i16 aptr;
	static i16 bptr;
	static i16 cptr;
	static i16 apred;
	static i16 bpred;
	static i16 cpred;
	static i16 mag;
	static i16 mv_x;
	static i16 mv_y;
	static i16 sum_x;
	static i16 sum_y;
	static i32 x_flag;
	static i16 res_shift;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_advance,
		my_state_compute,
		my_state_do_pred_x,
		my_state_do_pred_y,
		my_state_get_mag_x,
		my_state_get_mag_y,
		my_state_get_pred_p,
		my_state_get_pred_p1,
		my_state_get_pred_x,
		my_state_get_pred_x1,
		my_state_get_pred_x2,
		my_state_get_pred_y,
		my_state_get_pred_y1,
		my_state_get_pred_y2,
		my_state_get_res_x,
		my_state_get_res_x_a,
		my_state_get_res_x_b,
		my_state_get_res_x_c,
		my_state_get_res_x_d,
		my_state_get_res_x_e,
		my_state_get_res_y,
		my_state_get_res_y_a,
		my_state_get_res_y_b,
		my_state_get_res_y_c,
		my_state_get_res_y_d,
		my_state_get_res_y_e,
		my_state_geth,
		my_state_getw,
		my_state_read,
		my_state_write,
		my_state_write_y
	};

	static char *stateNames[] = {
		"advance",
		"compute",
		"do_pred_x",
		"do_pred_y",
		"get_mag_x",
		"get_mag_y",
		"get_pred_p",
		"get_pred_p1",
		"get_pred_x",
		"get_pred_x1",
		"get_pred_x2",
		"get_pred_y",
		"get_pred_y1",
		"get_pred_y2",
		"get_res_x",
		"get_res_x_a",
		"get_res_x_b",
		"get_res_x_c",
		"get_res_x_d",
		"get_res_x_e",
		"get_res_y",
		"get_res_y_a",
		"get_res_y_b",
		"get_res_y_c",
		"get_res_y_d",
		"get_res_y_e",
		"geth",
		"getw",
		"read",
		"write",
		"write_y"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_BTYPE() {
		index_BTYPE = decoder_parser_mvrecon_BTYPE->read_inds[1];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_parser_mvrecon_BTYPE, 1);
	}
	
	static void read_end_BTYPE() {
		decoder_parser_mvrecon_BTYPE->read_inds[1] = index_BTYPE;
	}
	static void read_MVIN() {
		index_MVIN = decoder_parser_mvrecon_MVIN->read_inds[0];
		numTokens_MVIN = index_MVIN + fifo_i16_get_num_tokens(decoder_parser_mvrecon_MVIN, 0);
	}
	
	static void read_end_MVIN() {
		decoder_parser_mvrecon_MVIN->read_inds[0] = index_MVIN;
	}
	static void read_A() {
		index_A = decoder_parser_mvrecon_A->read_inds[0];
		numTokens_A = index_A + fifo_i16_get_num_tokens(decoder_parser_mvrecon_A, 0);
	}
	
	static void read_end_A() {
		decoder_parser_mvrecon_A->read_inds[0] = index_A;
	}

	static void write_MV() {
		index_MV = (*decoder_parser_mvrecon_MV->write_ind);
		numFree_MV = index_MV + fifo_i16_get_room(decoder_parser_mvrecon_MV, NUM_READERS_MV);
	}
	
	static void write_end_MV() {
		(*decoder_parser_mvrecon_MV->write_ind) = index_MV;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i32 middle(i32 a, i32 b, i32 c);
	static i32 mvcalc(i32 pred, i32 mv_mag, i32 mag_shift);
	static i32 mvclip(i32 v);
	static i32 uvclip_1(i32 v);
	static i32 uvclip_4(i32 v);

	static i32 middle(i32 a, i32 b, i32 c) {
		i32 tmp_if;
	
		if (a < b) {
			if (a > c) {
				tmp_if = a;
			} else {
				if (b < c) {
					tmp_if = b;
				} else {
					tmp_if = c;
				}
			}
		} else {
			if (b > c) {
				tmp_if = b;
			} else {
				if (a < c) {
					tmp_if = a;
				} else {
					tmp_if = c;
				}
			}
		}
		return tmp_if;
	}
	static i32 mvcalc(i32 pred, i32 mv_mag, i32 mag_shift) {
		i16 local_mv_rsize;
		i64 tmp_if;
	
		local_mv_rsize = mv_rsize;
		if (local_mv_rsize == 0 || mv_mag == 0) {
			tmp_if = pred + mv_mag;
		} else {
			if (mv_mag < 0) {
				tmp_if = pred - mag_shift;
			} else {
				tmp_if = pred + mag_shift;
			}
		}
		return tmp_if;
	}
	static i32 mvclip(i32 v) {
		i16 local_mv_low;
		i64 tmp_if;
		i16 local_mv_range;
		i16 local_mv_high;
	
		local_mv_low = mv_low;
		if (v < local_mv_low) {
			local_mv_range = mv_range;
			tmp_if = v + local_mv_range;
		} else {
			local_mv_high = mv_high;
			if (v > local_mv_high) {
				local_mv_range = mv_range;
				tmp_if = v - local_mv_range;
			} else {
				tmp_if = v;
			}
		}
		return tmp_if;
	}
	static i32 uvclip_1(i32 v) {
		i32 vv;
		u8 tmp_if;
	
		vv = v >> 1;
		if ((v & 3) == 0) {
			tmp_if = 0;
		} else {
			tmp_if = 1;
		}
		return vv | tmp_if;
	}
	static i32 uvclip_4(i32 v) {
		i32 sign;
		i32 absv;
		i32 resv;
		i32 delta;
		i32 vv;
		i32 tmp_if;
	
		sign = v < 0;
		if (sign) {
			absv = -v;
		} else {
			absv = v;
		}
		resv = absv - ((absv >> 4) << 4);
		if (resv < 3) {
			delta = 0;
		} else {
			if (resv > 13) {
				delta = 2;
			} else {
				delta = 1;
			}
		}
		vv = ((absv >> 4) << 1) + delta;
		if (sign) {
			tmp_if = -vv;
		} else {
			tmp_if = vv;
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
	
		i16 cmd;
		i32 local_FCODE_MASK;
		i32 local_FCODE_SHIFT;
		i8 fcode;
		i16 local_mv_rsize;
		i16 local_mv_range;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_FCODE_MASK = ConstantsBtype_FCODE_MASK;
		local_FCODE_SHIFT = Constants_FCODE_SHIFT;
		fcode = (cmd & local_FCODE_MASK) >> local_FCODE_SHIFT;
		mbx = 0;
		top_edge = 1;
		comp = 0;
		if (fcode > 0) {
			mv_rsize = fcode - 1;
			local_mv_rsize = mv_rsize;
			mv_range = 1 << (local_mv_rsize + 5);
			local_mv_range = mv_range;
			mv_low = -local_mv_range;
			local_mv_range = mv_range;
			mv_high = local_mv_range - 1;
			local_mv_range = mv_range;
			mv_range = local_mv_range << 1;
		}
	
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
		ptr = 8;
	
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
	static i32 isSchedulable_read_noMotion() {
		i32 result;
		i16 cmd;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_NEWVOP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_NEWVOP = ConstantsBtype_NEWVOP;
		result = ((cmd & local_INTER) == 0 || (cmd & local_MOTION) == 0) && (cmd & local_NEWVOP) == 0;
		return result;
	}
	
	static void read_noMotion() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
		i16 local_ptr;
		i8 local_comp;
		i16 p;
		i8 local_Y_SELECT;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_ptr = ptr;
		local_comp = comp;
		p = local_ptr | local_comp;
		local_comp = comp;
		if (local_comp < 4) {
			buf[p] = 0;
			local_Y_SELECT = Y_SELECT;
			buf[p | local_Y_SELECT] = 0;
		}
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_motion() {
		i32 result;
		i16 cmd;
		i32 local_INTER;
		i32 local_MOTION;
		i32 local_NEWVOP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTER = ConstantsBtype_INTER;
		local_MOTION = ConstantsBtype_MOTION;
		local_NEWVOP = ConstantsBtype_NEWVOP;
		result = (cmd & local_INTER) != 0 && (cmd & local_MOTION) != 0 && (cmd & local_NEWVOP) == 0;
		return result;
	}
	
	static void read_motion() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
		i8 local_comp;
		i32 local_FOURMV;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_comp = comp;
		if (local_comp == 0) {
			local_FOURMV = Constants_FOURMV;
			fourmv = (cmd & local_FOURMV) != 0;
			sum_x = 0;
			sum_y = 0;
		}
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_compute_done() {
		i32 result;
		i8 local_comp;
		i32 local_fourmv;
	
		local_comp = comp;
		local_comp = comp;
		local_fourmv = fourmv;
		result = local_comp > 3 || local_comp != 0 && !local_fourmv;
		return result;
	}
	
	static void compute_done() {
	
		// Compute aligned port indexes
	
	
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_compute_start() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void compute_start() {
	
		// Compute aligned port indexes
		i32 index_aligned_A = index_A % SIZE_A;
	
		i16 a;
		i16 local_bptr;
		i16 local_cptr;
	
		a = tokens_A[(index_A + (0)) % SIZE_A];
		local_bptr = bptr;
		aptr = local_bptr;
		local_cptr = cptr;
		bptr = local_cptr;
		cptr = a;
	
		// Update ports indexes
		index_A += 1;
		
		read_end_A();
	}
	static i32 isSchedulable_get_pred() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_pred() {
	
		// Compute aligned port indexes
	
		i16 local_aptr;
		i16 t;
		i16 local_bpred;
		i16 local_cpred;
		i16 tmp_buf;
		i16 local_bptr;
		i16 local_cptr;
		i8 local_Y_SELECT;
	
		local_aptr = aptr;
		t = local_aptr;
		local_bpred = bpred;
		apred = local_bpred;
		local_cpred = cpred;
		bpred = local_cpred;
		tmp_buf = buf[t];
		cpred = tmp_buf;
		local_bptr = bptr;
		aptr = local_bptr;
		local_cptr = cptr;
		bptr = local_cptr;
		local_Y_SELECT = Y_SELECT;
		cptr = t | local_Y_SELECT;
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_do_pred() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void do_pred() {
	
		// Compute aligned port indexes
	
		i8 local_comp;
		i32 local_top_edge;
		i16 local_apred;
		i16 local_bpred;
		i16 local_cpred;
		i32 tmp_middle;
	
		local_comp = comp;
		local_top_edge = top_edge;
		if (local_comp >= 2 || !local_top_edge) {
			local_apred = apred;
			local_bpred = bpred;
			local_cpred = cpred;
			tmp_middle = middle(local_apred, local_bpred, local_cpred);
			apred = tmp_middle;
		}
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_get_mag() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_mag() {
	
		// Compute aligned port indexes
		i32 index_aligned_MVIN = index_MVIN % SIZE_MVIN;
	
		i16 m;
	
		m = tokens_MVIN[(index_MVIN + (0)) % SIZE_MVIN];
		mag = m;
	
		// Update ports indexes
		index_MVIN += 1;
		
		read_end_MVIN();
	}
	static i32 isSchedulable_get_residual_init() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_init() {
	
		// Compute aligned port indexes
	
		i16 local_mag;
	
		local_mag = mag;
		if (local_mag < 0) {
			local_mag = mag;
			res_shift = ~local_mag;
		} else {
			local_mag = mag;
			res_shift = local_mag - 1;
		}
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_get_residual_shift() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_shift() {
	
		// Compute aligned port indexes
	
		i16 local_mv_rsize;
		i16 count;
		i16 local_res_shift;
	
		local_mv_rsize = mv_rsize;
		count = local_mv_rsize;
		while (count > 0) {
			local_res_shift = res_shift;
			res_shift = local_res_shift << 1;
			count = count - 1;
		}
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_get_residual_adjust() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_adjust() {
	
		// Compute aligned port indexes
		i32 index_aligned_MVIN = index_MVIN % SIZE_MVIN;
	
		i16 s;
		i16 local_res_shift;
	
		s = tokens_MVIN[(index_MVIN + (0)) % SIZE_MVIN];
		local_res_shift = res_shift;
		res_shift = local_res_shift + s + 1;
	
		// Update ports indexes
		index_MVIN += 1;
		
		read_end_MVIN();
	}
	static i32 isSchedulable_get_residual_calc() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_calc() {
	
		// Compute aligned port indexes
	
		i16 local_apred;
		i16 local_mag;
		i16 local_res_shift;
		i32 tmp_mvcalc;
	
		local_apred = apred;
		local_mag = mag;
		local_res_shift = res_shift;
		tmp_mvcalc = mvcalc(local_apred, local_mag, local_res_shift);
		res_shift = tmp_mvcalc;
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_get_residual_clip() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_clip() {
	
		// Compute aligned port indexes
	
		i16 local_res_shift;
		i32 tmp_mvclip;
	
		local_res_shift = res_shift;
		tmp_mvclip = mvclip(local_res_shift);
		res_shift = tmp_mvclip;
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_get_residual_final() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_residual_final() {
	
		// Compute aligned port indexes
	
		i16 local_sum_x;
		i16 local_res_shift;
		i16 sum;
		i16 local_mv_y;
		i16 local_sum_y;
	
		local_sum_x = sum_x;
		local_res_shift = res_shift;
		sum = local_sum_x + local_res_shift;
		local_mv_y = mv_y;
		mv_x = local_mv_y;
		local_res_shift = res_shift;
		mv_y = local_res_shift;
		local_sum_y = sum_y;
		sum_x = local_sum_y;
		sum_y = sum;
		x_flag = 1;
	
		// Update ports indexes
		
	}
	static i32 isSchedulable_write_luma() {
		i32 result;
		i8 local_comp;
	
		local_comp = comp;
		result = local_comp < 4;
		return result;
	}
	
	static void write_luma() {
	
		// Compute aligned port indexes
		i32 index_aligned_MV = index_MV % SIZE_MV;
	
		i16 local_ptr;
		i8 local_comp;
		i16 p;
		i16 local_mv_x;
		i16 t;
		i32 local_x_flag;
		i8 tmp_if;
		i8 local_Y_SELECT;
		i16 local_mv_y;
	
		local_ptr = ptr;
		local_comp = comp;
		p = local_ptr | local_comp;
		local_mv_x = mv_x;
		t = local_mv_x;
		local_x_flag = x_flag;
		if (local_x_flag) {
			tmp_if = 0;
		} else {
			local_Y_SELECT = Y_SELECT;
			tmp_if = local_Y_SELECT;
		}
		local_mv_x = mv_x;
		buf[p | tmp_if] = local_mv_x;
		local_mv_y = mv_y;
		mv_x = local_mv_y;
		mv_y = t;
		local_x_flag = x_flag;
		x_flag = !local_x_flag;
		tokens_MV[(index_MV + (0)) % SIZE_MV] = t;
	
		// Update ports indexes
		index_MV += 1;
		
		write_end_MV();
	}
	static i32 isSchedulable_write_chroma() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void write_chroma() {
	
		// Compute aligned port indexes
		i32 index_aligned_MV = index_MV % SIZE_MV;
	
		i32 local_fourmv;
		i16 local_sum_x;
		i16 mv;
		i16 t;
		i16 local_sum_y;
	
		local_fourmv = fourmv;
		if (local_fourmv) {
			local_sum_x = sum_x;
			mv = uvclip_4(local_sum_x);
		} else {
			local_sum_x = sum_x;
			mv = uvclip_1(local_sum_x);
		}
		local_sum_x = sum_x;
		t = local_sum_x;
		local_sum_y = sum_y;
		sum_x = local_sum_y;
		sum_y = t;
		tokens_MV[(index_MV + (0)) % SIZE_MV] = mv;
	
		// Update ports indexes
		index_MV += 1;
		
		write_end_MV();
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
		i16 local_ptr;
		i32 local_BUF_SIZE;
	
		local_comp = comp;
		comp = local_comp + 1;
		local_comp = comp;
		if (local_comp == 6) {
			comp = 0;
			local_mbx = mbx;
			mbx = local_mbx + 1;
			local_mbx = mbx;
			local_mbwidth = mbwidth;
			if (local_mbx == local_mbwidth) {
				top_edge = 0;
			}
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
	
	void decoder_parser_mvrecon_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_read;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_parser_mvrecon_scheduler() {
		int i = 0;
		if(needInit){
			decoder_parser_mvrecon_initialize();
			needInit = 0;
		}
	
		read_BTYPE();
		read_MVIN();
		read_A();
		write_MV();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_advance:
			goto l_advance;
		case my_state_compute:
			goto l_compute;
		case my_state_do_pred_x:
			goto l_do_pred_x;
		case my_state_do_pred_y:
			goto l_do_pred_y;
		case my_state_get_mag_x:
			goto l_get_mag_x;
		case my_state_get_mag_y:
			goto l_get_mag_y;
		case my_state_get_pred_p:
			goto l_get_pred_p;
		case my_state_get_pred_p1:
			goto l_get_pred_p1;
		case my_state_get_pred_x:
			goto l_get_pred_x;
		case my_state_get_pred_x1:
			goto l_get_pred_x1;
		case my_state_get_pred_x2:
			goto l_get_pred_x2;
		case my_state_get_pred_y:
			goto l_get_pred_y;
		case my_state_get_pred_y1:
			goto l_get_pred_y1;
		case my_state_get_pred_y2:
			goto l_get_pred_y2;
		case my_state_get_res_x:
			goto l_get_res_x;
		case my_state_get_res_x_a:
			goto l_get_res_x_a;
		case my_state_get_res_x_b:
			goto l_get_res_x_b;
		case my_state_get_res_x_c:
			goto l_get_res_x_c;
		case my_state_get_res_x_d:
			goto l_get_res_x_d;
		case my_state_get_res_x_e:
			goto l_get_res_x_e;
		case my_state_get_res_y:
			goto l_get_res_y;
		case my_state_get_res_y_a:
			goto l_get_res_y_a;
		case my_state_get_res_y_b:
			goto l_get_res_y_b;
		case my_state_get_res_y_c:
			goto l_get_res_y_c;
		case my_state_get_res_y_d:
			goto l_get_res_y_d;
		case my_state_get_res_y_e:
			goto l_get_res_y_e;
		case my_state_geth:
			goto l_geth;
		case my_state_getw:
			goto l_getw;
		case my_state_read:
			goto l_read;
		case my_state_write:
			goto l_write;
		case my_state_write_y:
			goto l_write_y;
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
		l_compute:
			if (isSchedulable_compute_done()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_compute;
					goto finished;
				}
				compute_done();
				i++;
				goto l_write;
			} else if (numTokens_A - index_A >= 1 && isSchedulable_compute_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_compute;
					goto finished;
				}
				compute_start();
				i++;
				goto l_get_pred_p;
			} else {
				_FSM_state = my_state_compute;
				goto finished;
			}
		l_do_pred_x:
			if (isSchedulable_do_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_do_pred_x;
					goto finished;
				}
				do_pred();
				i++;
				goto l_get_mag_x;
			} else {
				_FSM_state = my_state_do_pred_x;
				goto finished;
			}
		l_do_pred_y:
			if (isSchedulable_do_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_do_pred_y;
					goto finished;
				}
				do_pred();
				i++;
				goto l_get_mag_y;
			} else {
				_FSM_state = my_state_do_pred_y;
				goto finished;
			}
		l_get_mag_x:
			if (numTokens_MVIN - index_MVIN >= 1 && isSchedulable_get_mag()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_mag_x;
					goto finished;
				}
				get_mag();
				i++;
				goto l_get_res_x;
			} else {
				_FSM_state = my_state_get_mag_x;
				goto finished;
			}
		l_get_mag_y:
			if (numTokens_MVIN - index_MVIN >= 1 && isSchedulable_get_mag()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_mag_y;
					goto finished;
				}
				get_mag();
				i++;
				goto l_get_res_y;
			} else {
				_FSM_state = my_state_get_mag_y;
				goto finished;
			}
		l_get_pred_p:
			if (numTokens_A - index_A >= 1 && isSchedulable_compute_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_p;
					goto finished;
				}
				compute_start();
				i++;
				goto l_get_pred_p1;
			} else {
				_FSM_state = my_state_get_pred_p;
				goto finished;
			}
		l_get_pred_p1:
			if (numTokens_A - index_A >= 1 && isSchedulable_compute_start()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_p1;
					goto finished;
				}
				compute_start();
				i++;
				goto l_get_pred_x;
			} else {
				_FSM_state = my_state_get_pred_p1;
				goto finished;
			}
		l_get_pred_x:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_x;
					goto finished;
				}
				get_pred();
				i++;
				goto l_get_pred_x1;
			} else {
				_FSM_state = my_state_get_pred_x;
				goto finished;
			}
		l_get_pred_x1:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_x1;
					goto finished;
				}
				get_pred();
				i++;
				goto l_get_pred_x2;
			} else {
				_FSM_state = my_state_get_pred_x1;
				goto finished;
			}
		l_get_pred_x2:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_x2;
					goto finished;
				}
				get_pred();
				i++;
				goto l_do_pred_x;
			} else {
				_FSM_state = my_state_get_pred_x2;
				goto finished;
			}
		l_get_pred_y:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_y;
					goto finished;
				}
				get_pred();
				i++;
				goto l_get_pred_y1;
			} else {
				_FSM_state = my_state_get_pred_y;
				goto finished;
			}
		l_get_pred_y1:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_y1;
					goto finished;
				}
				get_pred();
				i++;
				goto l_get_pred_y2;
			} else {
				_FSM_state = my_state_get_pred_y1;
				goto finished;
			}
		l_get_pred_y2:
			if (isSchedulable_get_pred()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_pred_y2;
					goto finished;
				}
				get_pred();
				i++;
				goto l_do_pred_y;
			} else {
				_FSM_state = my_state_get_pred_y2;
				goto finished;
			}
		l_get_res_x:
			if (isSchedulable_get_residual_init()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x;
					goto finished;
				}
				get_residual_init();
				i++;
				goto l_get_res_x_a;
			} else {
				_FSM_state = my_state_get_res_x;
				goto finished;
			}
		l_get_res_x_a:
			if (isSchedulable_get_residual_shift()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x_a;
					goto finished;
				}
				get_residual_shift();
				i++;
				goto l_get_res_x_b;
			} else {
				_FSM_state = my_state_get_res_x_a;
				goto finished;
			}
		l_get_res_x_b:
			if (numTokens_MVIN - index_MVIN >= 1 && isSchedulable_get_residual_adjust()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x_b;
					goto finished;
				}
				get_residual_adjust();
				i++;
				goto l_get_res_x_c;
			} else {
				_FSM_state = my_state_get_res_x_b;
				goto finished;
			}
		l_get_res_x_c:
			if (isSchedulable_get_residual_calc()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x_c;
					goto finished;
				}
				get_residual_calc();
				i++;
				goto l_get_res_x_d;
			} else {
				_FSM_state = my_state_get_res_x_c;
				goto finished;
			}
		l_get_res_x_d:
			if (isSchedulable_get_residual_clip()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x_d;
					goto finished;
				}
				get_residual_clip();
				i++;
				goto l_get_res_x_e;
			} else {
				_FSM_state = my_state_get_res_x_d;
				goto finished;
			}
		l_get_res_x_e:
			if (isSchedulable_get_residual_final()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_x_e;
					goto finished;
				}
				get_residual_final();
				i++;
				goto l_get_pred_y;
			} else {
				_FSM_state = my_state_get_res_x_e;
				goto finished;
			}
		l_get_res_y:
			if (isSchedulable_get_residual_init()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y;
					goto finished;
				}
				get_residual_init();
				i++;
				goto l_get_res_y_a;
			} else {
				_FSM_state = my_state_get_res_y;
				goto finished;
			}
		l_get_res_y_a:
			if (isSchedulable_get_residual_shift()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y_a;
					goto finished;
				}
				get_residual_shift();
				i++;
				goto l_get_res_y_b;
			} else {
				_FSM_state = my_state_get_res_y_a;
				goto finished;
			}
		l_get_res_y_b:
			if (numTokens_MVIN - index_MVIN >= 1 && isSchedulable_get_residual_adjust()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y_b;
					goto finished;
				}
				get_residual_adjust();
				i++;
				goto l_get_res_y_c;
			} else {
				_FSM_state = my_state_get_res_y_b;
				goto finished;
			}
		l_get_res_y_c:
			if (isSchedulable_get_residual_calc()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y_c;
					goto finished;
				}
				get_residual_calc();
				i++;
				goto l_get_res_y_d;
			} else {
				_FSM_state = my_state_get_res_y_c;
				goto finished;
			}
		l_get_res_y_d:
			if (isSchedulable_get_residual_clip()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y_d;
					goto finished;
				}
				get_residual_clip();
				i++;
				goto l_get_res_y_e;
			} else {
				_FSM_state = my_state_get_res_y_d;
				goto finished;
			}
		l_get_res_y_e:
			if (isSchedulable_get_residual_final()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_res_y_e;
					goto finished;
				}
				get_residual_final();
				i++;
				goto l_write;
			} else {
				_FSM_state = my_state_get_res_y_e;
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
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_noMotion()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_noMotion();
				i++;
				goto l_advance;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_read_motion()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_read;
					goto finished;
				}
				read_motion();
				i++;
				goto l_compute;
			} else {
				_FSM_state = my_state_read;
				goto finished;
			}
		l_write:
			if (isSchedulable_write_luma()) {
				int stop = 0;
				if (1 > SIZE_MV - index_MV + decoder_parser_mvrecon_MV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_write;
					goto finished;
				}
				write_luma();
				i++;
				goto l_write_y;
			} else if (isSchedulable_write_chroma()) {
				int stop = 0;
				if (1 > SIZE_MV - index_MV + decoder_parser_mvrecon_MV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_write;
					goto finished;
				}
				write_chroma();
				i++;
				goto l_write_y;
			} else {
				_FSM_state = my_state_write;
				goto finished;
			}
		l_write_y:
			if (isSchedulable_write_luma()) {
				int stop = 0;
				if (1 > SIZE_MV - index_MV + decoder_parser_mvrecon_MV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_write_y;
					goto finished;
				}
				write_luma();
				i++;
				goto l_advance;
			} else if (isSchedulable_write_chroma()) {
				int stop = 0;
				if (1 > SIZE_MV - index_MV + decoder_parser_mvrecon_MV->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_write_y;
					goto finished;
				}
				write_chroma();
				i++;
				goto l_advance;
			} else {
				_FSM_state = my_state_write_y;
				goto finished;
			}
	finished:
		
		read_end_BTYPE();
		read_end_MVIN();
		read_end_A();
		
		write_end_MV();
		
		
		return i;
	}
	
