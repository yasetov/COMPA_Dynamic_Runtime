	// Source file is "L/Research/src/fr/irisa/mpeg4/part2/sp/motion/Framebuffer.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_MV;
	static unsigned int numTokens_MV;
	#define SIZE_MV 512
	#define tokens_MV 	decoder_motion_framebuf_MV->contents
	
	
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_motion_framebuf_BTYPE->contents
	
	
	static unsigned int index_WD;
	static unsigned int numTokens_WD;
	#define SIZE_WD 512
	#define tokens_WD 	decoder_motion_framebuf_WD->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_halfpel;
	static unsigned int numFree_halfpel;
	#define NUM_READERS_halfpel 1
	#define SIZE_halfpel 512
	#define tokens_halfpel decoder_motion_framebuf_halfpel->contents
	static unsigned int index_RD;
	static unsigned int numFree_RD;
	#define NUM_READERS_RD 1
	#define SIZE_RD 512
	#define tokens_RD decoder_motion_framebuf_RD->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define Constants_ROUND_TYPE 32
	#define ConstantsBtype_INTRA 1024
	#define ConstantsBtype_MOTION 8
	#define SAMPLES_PER_MB 16
	#define SAMPLES_PER_MB_UV 8
	#define COMP_PER_MB 6
	#define MAXW_IN_SAMPLES 192
	#define MAXW_IN_SAMPLES_UV 96
	#define MAXH_IN_SAMPLES 160
	#define MAXH_IN_SAMPLES_UV 80
	#define MAX_FRAME_SIZE 30720
	#define MAX_FRAME_SIZE_UV 7680
	#define HALF_SEARCH 1
	#define EXTRA_SPACE 3072
	#define EXTRA_SPACE_UV 768
	#define BUFFER_SIZE 33792
	#define BUFFER_SIZE_UV 8448
	static i16 frameBuffer[33792];
	static i16 frameBuffer_U[8448];
	static i16 frameBuffer_V[8448];
	static i32 vop_width = 0;
	static i32 vop_height = 0;
	static i32 vop_width_in_samples;
	static i32 vop_height_in_samples;
	static i32 vop_rounding_mode;
	static i32 max_outstanding_writes;
	static i32 start_of_frame = -1;
	static i32 vop_width_in_samples_U;
	static i32 vop_height_in_samples_U;
	static i32 start_of_frame_U = -1;
	static i16 mvx;
	static i16 mvy;
	static i32 mbx_read = 0;
	static i32 mby_read = 0;
	static i32 outstanding_writes = 0;
	static i32 mb_write_ptr = 0;
	static i32 mbx_write = 0;
	static i32 mb_write_ptr_chroma = 0;
	static i32 comp_so_far = 0;
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_cmd,
		my_state_get_w_h,
		my_state_read_fb,
		my_state_read_mv
	};

	static char *stateNames[] = {
		"cmd",
		"get_w_h",
		"read_fb",
		"read_mv"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_MV() {
		index_MV = decoder_motion_framebuf_MV->read_inds[0];
		numTokens_MV = index_MV + fifo_i16_get_num_tokens(decoder_motion_framebuf_MV, 0);
	}
	
	static void read_end_MV() {
		decoder_motion_framebuf_MV->read_inds[0] = index_MV;
	}
	static void read_BTYPE() {
		index_BTYPE = decoder_motion_framebuf_BTYPE->read_inds[5];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_motion_framebuf_BTYPE, 5);
	}
	
	static void read_end_BTYPE() {
		decoder_motion_framebuf_BTYPE->read_inds[5] = index_BTYPE;
	}
	static void read_WD() {
		index_WD = decoder_motion_framebuf_WD->read_inds[0];
		numTokens_WD = index_WD + fifo_i16_get_num_tokens(decoder_motion_framebuf_WD, 0);
	}
	
	static void read_end_WD() {
		decoder_motion_framebuf_WD->read_inds[0] = index_WD;
	}

	static void write_halfpel() {
		index_halfpel = (*decoder_motion_framebuf_halfpel->write_ind);
		numFree_halfpel = index_halfpel + fifo_i8_get_room(decoder_motion_framebuf_halfpel, NUM_READERS_halfpel);
	}
	
	static void write_end_halfpel() {
		(*decoder_motion_framebuf_halfpel->write_ind) = index_halfpel;
	}
	static void write_RD() {
		index_RD = (*decoder_motion_framebuf_RD->write_ind);
		numFree_RD = index_RD + fifo_i16_get_room(decoder_motion_framebuf_RD, NUM_READERS_RD);
	}
	
	static void write_end_RD() {
		(*decoder_motion_framebuf_RD->write_ind) = index_RD;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static void advance_block();

	static void advance_block() {
		i32 local_outstanding_writes;
		i32 local_comp_so_far;
		i32 local_COMP_PER_MB;
		i32 local_mbx_read;
		i32 local_vop_width;
		i32 local_mby_read;
	
		local_outstanding_writes = outstanding_writes;
		outstanding_writes = local_outstanding_writes + 1;
		local_comp_so_far = comp_so_far;
		comp_so_far = local_comp_so_far + 1;
		local_comp_so_far = comp_so_far;
		local_COMP_PER_MB = COMP_PER_MB;
		if (local_comp_so_far == local_COMP_PER_MB) {
			local_mbx_read = mbx_read;
			mbx_read = local_mbx_read + 1;
			comp_so_far = 0;
			local_mbx_read = mbx_read;
			local_vop_width = vop_width;
			if (local_mbx_read == local_vop_width) {
				mbx_read = 0;
				local_mby_read = mby_read;
				mby_read = local_mby_read + 1;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// Actions
	static i32 isSchedulable_cmd_newVop() {
		i32 result;
		i16 cmd;
		i32 local_NEWVOP;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_NEWVOP = ConstantsBtype_NEWVOP;
		result = (cmd & local_NEWVOP) != 0;
		return result;
	}
	
	static void cmd_newVop() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
		i32 local_ROUND_TYPE;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_ROUND_TYPE = Constants_ROUND_TYPE;
		if ((cmd & local_ROUND_TYPE) != 0) {
			vop_rounding_mode = 1;
		} else {
			vop_rounding_mode = 0;
		}
		comp_so_far = 0;
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_get_w_h() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void get_w_h() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 w;
		i16 h;
		i32 local_vop_height;
		i32 local_SAMPLES_PER_MB;
		i32 local_MAXW_IN_SAMPLES;
		i32 last_frame_size;
		i32 local_SAMPLES_PER_MB_UV;
		i32 local_MAXW_IN_SAMPLES_UV;
		i32 last_frame_size_UV;
		i32 local_HALF_SEARCH;
		i32 local_vop_width;
		i32 local_COMP_PER_MB;
		i32 local_start_of_frame;
		i32 local_BUFFER_SIZE;
		i32 local_start_of_frame_U;
		i32 local_BUFFER_SIZE_UV;
	
		w = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		h = tokens_BTYPE[(index_BTYPE + (1)) % SIZE_BTYPE];
		local_vop_height = vop_height;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
		last_frame_size = local_vop_height * local_SAMPLES_PER_MB * local_MAXW_IN_SAMPLES;
		local_vop_height = vop_height;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
		last_frame_size_UV = local_vop_height * local_SAMPLES_PER_MB_UV * local_MAXW_IN_SAMPLES_UV;
		vop_width = w;
		vop_height = h;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		vop_width_in_samples = w * local_SAMPLES_PER_MB;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		vop_height_in_samples = h * local_SAMPLES_PER_MB;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		vop_width_in_samples_U = w * local_SAMPLES_PER_MB_UV;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		vop_height_in_samples_U = h * local_SAMPLES_PER_MB_UV;
		local_vop_height = vop_height;
		local_HALF_SEARCH = HALF_SEARCH;
		local_vop_width = vop_width;
		local_COMP_PER_MB = COMP_PER_MB;
		max_outstanding_writes = (local_vop_height - local_HALF_SEARCH) * local_vop_width * local_COMP_PER_MB;
		mbx_read = 0;
		mby_read = 0;
		if (last_frame_size != 0) {
			local_start_of_frame = start_of_frame;
			start_of_frame = local_start_of_frame + last_frame_size;
		} else {
			local_BUFFER_SIZE = BUFFER_SIZE;
			local_vop_height = vop_height;
			local_SAMPLES_PER_MB = SAMPLES_PER_MB;
			local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
			start_of_frame = local_BUFFER_SIZE - local_vop_height * local_SAMPLES_PER_MB * local_MAXW_IN_SAMPLES;
		}
		if (last_frame_size_UV != 0) {
			local_start_of_frame_U = start_of_frame_U;
			start_of_frame_U = local_start_of_frame_U + last_frame_size_UV;
		} else {
			local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
			local_vop_height = vop_height;
			local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
			local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
			start_of_frame_U = local_BUFFER_SIZE_UV - local_vop_height * local_SAMPLES_PER_MB_UV * local_MAXW_IN_SAMPLES_UV;
		}
		local_start_of_frame = start_of_frame;
		local_BUFFER_SIZE = BUFFER_SIZE;
		if (local_start_of_frame >= local_BUFFER_SIZE) {
			local_start_of_frame = start_of_frame;
			local_BUFFER_SIZE = BUFFER_SIZE;
			start_of_frame = local_start_of_frame - local_BUFFER_SIZE;
		}
		local_start_of_frame_U = start_of_frame_U;
		local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
		if (local_start_of_frame_U >= local_BUFFER_SIZE_UV) {
			local_start_of_frame_U = start_of_frame_U;
			local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
			start_of_frame_U = local_start_of_frame_U - local_BUFFER_SIZE_UV;
		}
	
		// Update ports indexes
		index_BTYPE += 2;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_cmd_noMotion() {
		i32 result;
		i16 cmd;
		i32 local_INTRA;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTRA = ConstantsBtype_INTRA;
		result = (cmd & local_INTRA) != 0;
		return result;
	}
	
	static void cmd_noMotion() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		advance_block();
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_cmd_motion() {
		i32 result;
		i16 cmd;
		i32 local_MOTION;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_MOTION = ConstantsBtype_MOTION;
		result = (cmd & local_MOTION) != 0;
		return result;
	}
	
	static void cmd_motion() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_read_mv() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void read_mv() {
	
		// Compute aligned port indexes
		i32 index_aligned_MV = index_MV % SIZE_MV;
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
	
		i16 x;
		i16 y;
		i32 local_vop_rounding_mode;
		i8 flags;
	
		x = tokens_MV[(index_MV + (0)) % SIZE_MV];
		y = tokens_MV[(index_MV + (1)) % SIZE_MV];
		local_vop_rounding_mode = vop_rounding_mode;
		flags = ((x & 1) << 2) + ((y & 1) << 1) + local_vop_rounding_mode;
		mvx = x >> 1;
		mvy = y >> 1;
		tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel] = flags;
	
		// Update ports indexes
		index_MV += 2;
		index_halfpel += 1;
		
		read_end_MV();
		write_end_halfpel();
	}
	static i32 isSchedulable_cmd_neither() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void cmd_neither() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
		i32 index_aligned_halfpel = index_halfpel % SIZE_halfpel;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		mvx = 0;
		mvy = 0;
		tokens_halfpel[(index_halfpel + (0)) % SIZE_halfpel] = 0;
	
		// Update ports indexes
		index_BTYPE += 1;
		index_halfpel += 1;
		
		read_end_BTYPE();
		write_end_halfpel();
	}
	static i32 isSchedulable_read_fb_Y() {
		i32 result;
		i32 local_outstanding_writes;
		i32 local_max_outstanding_writes;
		i32 local_comp_so_far;
	
		local_outstanding_writes = outstanding_writes;
		local_max_outstanding_writes = max_outstanding_writes;
		local_comp_so_far = comp_so_far;
		result = local_outstanding_writes < local_max_outstanding_writes && local_comp_so_far < 4;
		return result;
	}
	
	static void read_fb_Y() {
	
		// Compute aligned port indexes
		i32 index_aligned_RD = index_RD % SIZE_RD;
	
		i32 local_mbx_read;
		i32 local_SAMPLES_PER_MB;
		i32 local_comp_so_far;
		i16 local_mvx;
		i32 x;
		i32 local_mby_read;
		i16 local_mvy;
		i32 y;
		i32 clip_left;
		i32 clip_right;
		i32 clip_x;
		i32 local_vop_height_in_samples;
		i32 clip_y;
		i32 start_of_row;
		i32 addr;
		i32 i;
		i32 end_i;
		i32 temp;
		i32 local_vop_width_in_samples;
		i32 local_start_of_frame;
		i32 local_MAXW_IN_SAMPLES;
		i32 local_BUFFER_SIZE;
		i16 tmp_frameBuffer;
	
		local_mbx_read = mbx_read;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		local_comp_so_far = comp_so_far;
		local_mvx = mvx;
		x = local_mbx_read * local_SAMPLES_PER_MB + 8 * (local_comp_so_far & 1) + local_mvx;
		local_mby_read = mby_read;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		local_comp_so_far = comp_so_far;
		local_mvy = mvy;
		y = local_mby_read * local_SAMPLES_PER_MB + 8 * (local_comp_so_far >> 1) + local_mvy;
		clip_left = 0;
		clip_right = 0;
		clip_x = x;
		if (y < 0) {
			clip_y = 0;
		} else {
			local_vop_height_in_samples = vop_height_in_samples;
			if (y < local_vop_height_in_samples) {
				clip_y = y;
			} else {
				local_vop_height_in_samples = vop_height_in_samples;
				clip_y = local_vop_height_in_samples - 1;
			}
		}
		if (x < 0) {
			clip_x = 0;
			if (x < -9) {
				clip_left = 9;
			} else {
				clip_left = -x;
			}
		} else {
			local_vop_width_in_samples = vop_width_in_samples;
			if (x + 9 > local_vop_width_in_samples) {
				local_vop_width_in_samples = vop_width_in_samples;
				if (x >= local_vop_width_in_samples) {
					local_vop_width_in_samples = vop_width_in_samples;
					clip_x = local_vop_width_in_samples - 1;
					clip_right = 8;
				} else {
					local_vop_width_in_samples = vop_width_in_samples;
					clip_right = x + 9 - local_vop_width_in_samples;
				}
			}
		}
		local_start_of_frame = start_of_frame;
		local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
		start_of_row = local_start_of_frame + clip_y * local_MAXW_IN_SAMPLES + clip_x;
		local_BUFFER_SIZE = BUFFER_SIZE;
		if (start_of_row >= local_BUFFER_SIZE) {
			local_BUFFER_SIZE = BUFFER_SIZE;
			start_of_row = start_of_row - local_BUFFER_SIZE;
		}
		i = 0;
		while (i < 81) {
			if (clip_left != 0) {
				end_i = i + clip_left;
				temp = frameBuffer[start_of_row];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			addr = start_of_row;
			end_i = i + 9 - clip_left - clip_right;
			while (i < end_i) {
				tmp_frameBuffer = frameBuffer[addr];
				tokens_RD[(index_RD + (i)) % SIZE_RD] = tmp_frameBuffer;
				i = i + 1;
				addr = addr + 1;
			}
			if (clip_right != 0) {
				end_i = i + clip_right;
				temp = frameBuffer[addr - 1];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			y = y + 1;
			local_vop_height_in_samples = vop_height_in_samples;
			if (y > 0 && y < local_vop_height_in_samples) {
				local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
				start_of_row = start_of_row + local_MAXW_IN_SAMPLES;
				local_BUFFER_SIZE = BUFFER_SIZE;
				if (start_of_row >= local_BUFFER_SIZE) {
					local_BUFFER_SIZE = BUFFER_SIZE;
					start_of_row = start_of_row - local_BUFFER_SIZE;
				}
			}
		}
		advance_block();
	
		// Update ports indexes
		index_RD += 81;
		
		write_end_RD();
	}
	static i32 isSchedulable_read_fb_U() {
		i32 result;
		i32 local_outstanding_writes;
		i32 local_max_outstanding_writes;
		i32 local_comp_so_far;
	
		local_outstanding_writes = outstanding_writes;
		local_max_outstanding_writes = max_outstanding_writes;
		local_comp_so_far = comp_so_far;
		result = local_outstanding_writes < local_max_outstanding_writes && local_comp_so_far == 4;
		return result;
	}
	
	static void read_fb_U() {
	
		// Compute aligned port indexes
		i32 index_aligned_RD = index_RD % SIZE_RD;
	
		i32 local_mbx_read;
		i32 local_SAMPLES_PER_MB_UV;
		i16 local_mvx;
		i32 x;
		i32 local_mby_read;
		i16 local_mvy;
		i32 y;
		i32 clip_left;
		i32 clip_right;
		i32 clip_x;
		i32 local_vop_height_in_samples_U;
		i32 clip_y;
		i32 start_of_row;
		i32 addr;
		i32 i;
		i32 end_i;
		i32 temp;
		i32 local_vop_width_in_samples_U;
		i32 local_start_of_frame_U;
		i32 local_MAXW_IN_SAMPLES_UV;
		i32 local_BUFFER_SIZE_UV;
		i16 tmp_frameBuffer_U;
	
		local_mbx_read = mbx_read;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		local_mvx = mvx;
		x = local_mbx_read * local_SAMPLES_PER_MB_UV + local_mvx;
		local_mby_read = mby_read;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		local_mvy = mvy;
		y = local_mby_read * local_SAMPLES_PER_MB_UV + local_mvy;
		clip_left = 0;
		clip_right = 0;
		clip_x = x;
		if (y < 0) {
			clip_y = 0;
		} else {
			local_vop_height_in_samples_U = vop_height_in_samples_U;
			if (y < local_vop_height_in_samples_U) {
				clip_y = y;
			} else {
				local_vop_height_in_samples_U = vop_height_in_samples_U;
				clip_y = local_vop_height_in_samples_U - 1;
			}
		}
		if (x < 0) {
			clip_x = 0;
			if (x < -9) {
				clip_left = 9;
			} else {
				clip_left = -x;
			}
		} else {
			local_vop_width_in_samples_U = vop_width_in_samples_U;
			if (x + 9 > local_vop_width_in_samples_U) {
				local_vop_width_in_samples_U = vop_width_in_samples_U;
				if (x >= local_vop_width_in_samples_U) {
					local_vop_width_in_samples_U = vop_width_in_samples_U;
					clip_x = local_vop_width_in_samples_U - 1;
					clip_right = 8;
				} else {
					local_vop_width_in_samples_U = vop_width_in_samples_U;
					clip_right = x + 9 - local_vop_width_in_samples_U;
				}
			}
		}
		local_start_of_frame_U = start_of_frame_U;
		local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
		start_of_row = local_start_of_frame_U + clip_y * local_MAXW_IN_SAMPLES_UV + clip_x;
		local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
		if (start_of_row >= local_BUFFER_SIZE_UV) {
			local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
			start_of_row = start_of_row - local_BUFFER_SIZE_UV;
		}
		i = 0;
		while (i < 81) {
			if (clip_left != 0) {
				end_i = i + clip_left;
				temp = frameBuffer_U[start_of_row];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			addr = start_of_row;
			end_i = i + 9 - clip_left - clip_right;
			while (i < end_i) {
				tmp_frameBuffer_U = frameBuffer_U[addr];
				tokens_RD[(index_RD + (i)) % SIZE_RD] = tmp_frameBuffer_U;
				i = i + 1;
				addr = addr + 1;
			}
			if (clip_right != 0) {
				end_i = i + clip_right;
				temp = frameBuffer_U[addr - 1];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			y = y + 1;
			local_vop_height_in_samples_U = vop_height_in_samples_U;
			if (y > 0 && y < local_vop_height_in_samples_U) {
				local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
				start_of_row = start_of_row + local_MAXW_IN_SAMPLES_UV;
				local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
				if (start_of_row >= local_BUFFER_SIZE_UV) {
					local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
					start_of_row = start_of_row - local_BUFFER_SIZE_UV;
				}
			}
		}
		advance_block();
	
		// Update ports indexes
		index_RD += 81;
		
		write_end_RD();
	}
	static i32 isSchedulable_read_fb_V() {
		i32 result;
		i32 local_outstanding_writes;
		i32 local_max_outstanding_writes;
	
		local_outstanding_writes = outstanding_writes;
		local_max_outstanding_writes = max_outstanding_writes;
		result = local_outstanding_writes < local_max_outstanding_writes;
		return result;
	}
	
	static void read_fb_V() {
	
		// Compute aligned port indexes
		i32 index_aligned_RD = index_RD % SIZE_RD;
	
		i32 local_mbx_read;
		i32 local_SAMPLES_PER_MB_UV;
		i16 local_mvx;
		i32 x;
		i32 local_mby_read;
		i16 local_mvy;
		i32 y;
		i32 clip_left;
		i32 clip_right;
		i32 clip_x;
		i32 local_vop_height_in_samples_U;
		i32 clip_y;
		i32 start_of_row;
		i32 addr;
		i32 i;
		i32 end_i;
		i32 temp;
		i32 local_vop_width_in_samples_U;
		i32 local_start_of_frame_U;
		i32 local_MAXW_IN_SAMPLES_UV;
		i32 local_BUFFER_SIZE_UV;
		i16 tmp_frameBuffer_V;
	
		local_mbx_read = mbx_read;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		local_mvx = mvx;
		x = local_mbx_read * local_SAMPLES_PER_MB_UV + local_mvx;
		local_mby_read = mby_read;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		local_mvy = mvy;
		y = local_mby_read * local_SAMPLES_PER_MB_UV + local_mvy;
		clip_left = 0;
		clip_right = 0;
		clip_x = x;
		if (y < 0) {
			clip_y = 0;
		} else {
			local_vop_height_in_samples_U = vop_height_in_samples_U;
			if (y < local_vop_height_in_samples_U) {
				clip_y = y;
			} else {
				local_vop_height_in_samples_U = vop_height_in_samples_U;
				clip_y = local_vop_height_in_samples_U - 1;
			}
		}
		if (x < 0) {
			clip_x = 0;
			if (x < -9) {
				clip_left = 9;
			} else {
				clip_left = -x;
			}
		} else {
			local_vop_width_in_samples_U = vop_width_in_samples_U;
			if (x + 9 > local_vop_width_in_samples_U) {
				local_vop_width_in_samples_U = vop_width_in_samples_U;
				if (x >= local_vop_width_in_samples_U) {
					local_vop_width_in_samples_U = vop_width_in_samples_U;
					clip_x = local_vop_width_in_samples_U - 1;
					clip_right = 8;
				} else {
					local_vop_width_in_samples_U = vop_width_in_samples_U;
					clip_right = x + 9 - local_vop_width_in_samples_U;
				}
			}
		}
		local_start_of_frame_U = start_of_frame_U;
		local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
		start_of_row = local_start_of_frame_U + clip_y * local_MAXW_IN_SAMPLES_UV + clip_x;
		local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
		if (start_of_row >= local_BUFFER_SIZE_UV) {
			local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
			start_of_row = start_of_row - local_BUFFER_SIZE_UV;
		}
		i = 0;
		while (i < 81) {
			if (clip_left != 0) {
				end_i = i + clip_left;
				temp = frameBuffer_V[start_of_row];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			addr = start_of_row;
			end_i = i + 9 - clip_left - clip_right;
			while (i < end_i) {
				tmp_frameBuffer_V = frameBuffer_V[addr];
				tokens_RD[(index_RD + (i)) % SIZE_RD] = tmp_frameBuffer_V;
				i = i + 1;
				addr = addr + 1;
			}
			if (clip_right != 0) {
				end_i = i + clip_right;
				temp = frameBuffer_V[addr - 1];
				while (i < end_i) {
					tokens_RD[(index_RD + (i)) % SIZE_RD] = temp;
					i = i + 1;
				}
			}
			y = y + 1;
			local_vop_height_in_samples_U = vop_height_in_samples_U;
			if (y > 0 && y < local_vop_height_in_samples_U) {
				local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
				start_of_row = start_of_row + local_MAXW_IN_SAMPLES_UV;
				local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
				if (start_of_row >= local_BUFFER_SIZE_UV) {
					local_BUFFER_SIZE_UV = BUFFER_SIZE_UV;
					start_of_row = start_of_row - local_BUFFER_SIZE_UV;
				}
			}
		}
		advance_block();
	
		// Update ports indexes
		index_RD += 81;
		
		write_end_RD();
	}
	static i32 isSchedulable_untagged_0() {
		i32 result;
		i32 local_outstanding_writes;
	
		local_outstanding_writes = outstanding_writes;
		result = local_outstanding_writes >= 6;
		return result;
	}
	
	static void untagged_0() {
	
		// Compute aligned port indexes
		i32 index_aligned_WD = index_WD % SIZE_WD;
	
		i32 local_mb_write_ptr;
		i32 addr;
		i32 local_mb_write_ptr_chroma;
		i32 addr_chroma;
		i32 y;
		i32 x;
		i16 tmp_WD;
		i16 tmp_WD0;
		i32 local_MAXW_IN_SAMPLES;
		i16 tmp_WD1;
		i16 tmp_WD2;
		i16 tmp_WD3;
		i16 tmp_WD4;
		i32 local_MAXW_IN_SAMPLES_UV;
		i32 local_outstanding_writes;
		i32 local_mbx_write;
		i32 local_SAMPLES_PER_MB;
		i32 local_SAMPLES_PER_MB_UV;
		i32 local_vop_width;
		i32 local_BUFFER_SIZE;
	
		local_mb_write_ptr = mb_write_ptr;
		addr = local_mb_write_ptr;
		local_mb_write_ptr_chroma = mb_write_ptr_chroma;
		addr_chroma = local_mb_write_ptr_chroma;
		y = 0;
		while (y <= 7) {
			x = 0;
			while (x <= 7) {
				tmp_WD = tokens_WD[(index_WD + (8 * y + x)) % SIZE_WD];
				frameBuffer[addr + x] = tmp_WD;
				tmp_WD0 = tokens_WD[(index_WD + (8 * y + x + 64)) % SIZE_WD];
				frameBuffer[addr + x + 8] = tmp_WD0;
				local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
				tmp_WD1 = tokens_WD[(index_WD + (8 * y + x + 128)) % SIZE_WD];
				frameBuffer[addr + x + 8 * local_MAXW_IN_SAMPLES] = tmp_WD1;
				local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
				tmp_WD2 = tokens_WD[(index_WD + (8 * y + x + 192)) % SIZE_WD];
				frameBuffer[addr + x + 8 + 8 * local_MAXW_IN_SAMPLES] = tmp_WD2;
				tmp_WD3 = tokens_WD[(index_WD + (8 * y + x + 256)) % SIZE_WD];
				frameBuffer_U[addr_chroma + x] = tmp_WD3;
				tmp_WD4 = tokens_WD[(index_WD + (8 * y + x + 320)) % SIZE_WD];
				frameBuffer_V[addr_chroma + x] = tmp_WD4;
				x = x + 1;
			}
			local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
			addr = addr + local_MAXW_IN_SAMPLES;
			local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
			addr_chroma = addr_chroma + local_MAXW_IN_SAMPLES_UV;
			y = y + 1;
		}
		local_outstanding_writes = outstanding_writes;
		outstanding_writes = local_outstanding_writes - 6;
		local_mbx_write = mbx_write;
		mbx_write = local_mbx_write + 1;
		local_mb_write_ptr = mb_write_ptr;
		local_SAMPLES_PER_MB = SAMPLES_PER_MB;
		mb_write_ptr = local_mb_write_ptr + local_SAMPLES_PER_MB;
		local_mb_write_ptr_chroma = mb_write_ptr_chroma;
		local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
		mb_write_ptr_chroma = local_mb_write_ptr_chroma + local_SAMPLES_PER_MB_UV;
		local_mbx_write = mbx_write;
		local_vop_width = vop_width;
		if (local_mbx_write == local_vop_width) {
			mbx_write = 0;
			local_mb_write_ptr = mb_write_ptr;
			local_SAMPLES_PER_MB = SAMPLES_PER_MB;
			local_vop_width = vop_width;
			local_SAMPLES_PER_MB = SAMPLES_PER_MB;
			local_MAXW_IN_SAMPLES = MAXW_IN_SAMPLES;
			mb_write_ptr = local_mb_write_ptr - local_SAMPLES_PER_MB * local_vop_width + local_SAMPLES_PER_MB * local_MAXW_IN_SAMPLES;
			local_mb_write_ptr_chroma = mb_write_ptr_chroma;
			local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
			local_vop_width = vop_width;
			local_SAMPLES_PER_MB_UV = SAMPLES_PER_MB_UV;
			local_MAXW_IN_SAMPLES_UV = MAXW_IN_SAMPLES_UV;
			mb_write_ptr_chroma = local_mb_write_ptr_chroma - local_SAMPLES_PER_MB_UV * local_vop_width + local_SAMPLES_PER_MB_UV * local_MAXW_IN_SAMPLES_UV;
			local_mb_write_ptr = mb_write_ptr;
			local_BUFFER_SIZE = BUFFER_SIZE;
			if (local_mb_write_ptr == local_BUFFER_SIZE) {
				mb_write_ptr = 0;
				mb_write_ptr_chroma = 0;
			}
		}
	
		// Update ports indexes
		index_WD += 384;
		
		read_end_WD();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_motion_framebuf_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_cmd;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	int decoder_motion_framebuf_outside_FSM_scheduler() {
		int i = 0;
		while (1) {
			// Action loop
			if (numTokens_WD - index_WD >= 384 && isSchedulable_untagged_0()) {
				int stop = 0;
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
		// no read_end/write_end here!
		return i;
	}
	
	int decoder_motion_framebuf_scheduler() {
		int i = 0;
		if(needInit){
			decoder_motion_framebuf_initialize();
			needInit = 0;
		}
	
		read_MV();
		read_BTYPE();
		read_WD();
		write_halfpel();
		write_RD();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_cmd:
			goto l_cmd;
		case my_state_get_w_h:
			goto l_get_w_h;
		case my_state_read_fb:
			goto l_read_fb;
		case my_state_read_mv:
			goto l_read_mv;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_cmd:
			i += decoder_motion_framebuf_outside_FSM_scheduler();
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_newVop()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_newVop();
				i++;
				goto l_get_w_h;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_noMotion()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_noMotion();
				i++;
				goto l_cmd;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_motion()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_motion();
				i++;
				goto l_read_mv;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_neither()) {
				int stop = 0;
				if (1 > SIZE_halfpel - index_halfpel + decoder_motion_framebuf_halfpel->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_neither();
				i++;
				goto l_read_fb;
			} else {
				_FSM_state = my_state_cmd;
				goto finished;
			}
		l_get_w_h:
			i += decoder_motion_framebuf_outside_FSM_scheduler();
			if (numTokens_BTYPE - index_BTYPE >= 2 && isSchedulable_get_w_h()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_get_w_h;
					goto finished;
				}
				get_w_h();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_get_w_h;
				goto finished;
			}
		l_read_fb:
			i += decoder_motion_framebuf_outside_FSM_scheduler();
			if (isSchedulable_read_fb_Y()) {
				int stop = 0;
				if (81 > SIZE_RD - index_RD + decoder_motion_framebuf_RD->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read_fb;
					goto finished;
				}
				read_fb_Y();
				i++;
				goto l_cmd;
			} else if (isSchedulable_read_fb_U()) {
				int stop = 0;
				if (81 > SIZE_RD - index_RD + decoder_motion_framebuf_RD->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read_fb;
					goto finished;
				}
				read_fb_U();
				i++;
				goto l_cmd;
			} else if (isSchedulable_read_fb_V()) {
				int stop = 0;
				if (81 > SIZE_RD - index_RD + decoder_motion_framebuf_RD->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read_fb;
					goto finished;
				}
				read_fb_V();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_read_fb;
				goto finished;
			}
		l_read_mv:
			i += decoder_motion_framebuf_outside_FSM_scheduler();
			if (numTokens_MV - index_MV >= 2 && isSchedulable_read_mv()) {
				int stop = 0;
				if (1 > SIZE_halfpel - index_halfpel + decoder_motion_framebuf_halfpel->read_inds[0]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_read_mv;
					goto finished;
				}
				read_mv();
				i++;
				goto l_read_fb;
			} else {
				_FSM_state = my_state_read_mv;
				goto finished;
			}
	finished:
		
		read_end_MV();
		read_end_BTYPE();
		read_end_WD();
		
		write_end_halfpel();
		write_end_RD();
		
		
		return i;
	}
	
