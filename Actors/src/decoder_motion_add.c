	// Source file is "L/Research/src/fr/irisa/mpeg4/part2/sp/motion/Algo_Add.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_MOT;
	static unsigned int numTokens_MOT;
	#define SIZE_MOT 512
	#define tokens_MOT 	decoder_motion_add_MOT->contents
	
	
	static unsigned int index_TEX;
	static unsigned int numTokens_TEX;
	#define SIZE_TEX 512
	#define tokens_TEX 	decoder_motion_add_TEX->contents
	
	
	static unsigned int index_BTYPE;
	static unsigned int numTokens_BTYPE;
	#define SIZE_BTYPE 512
	#define tokens_BTYPE 	decoder_motion_add_BTYPE->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_VID;
	static unsigned int numFree_VID;
	#define NUM_READERS_VID 2
	#define SIZE_VID 512
	#define tokens_VID decoder_motion_add_VID->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define ConstantsBtype_NEWVOP 2048
	#define ConstantsBtype_INTRA 1024
	#define Constants_ACCODED 2
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Initial FSM state of the actor
	enum states {
		my_state_cmd,
		my_state_combine,
		my_state_motion,
		my_state_skiph,
		my_state_skipw,
		my_state_texture
	};

	static char *stateNames[] = {
		"cmd",
		"combine",
		"motion",
		"skiph",
		"skipw",
		"texture"
	};

	static enum states _FSM_state;

	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_MOT() {
		index_MOT = decoder_motion_add_MOT->read_inds[0];
		numTokens_MOT = index_MOT + fifo_i16_get_num_tokens(decoder_motion_add_MOT, 0);
	}
	
	static void read_end_MOT() {
		decoder_motion_add_MOT->read_inds[0] = index_MOT;
	}
	static void read_TEX() {
		index_TEX = decoder_motion_add_TEX->read_inds[0];
		numTokens_TEX = index_TEX + fifo_i16_get_num_tokens(decoder_motion_add_TEX, 0);
	}
	
	static void read_end_TEX() {
		decoder_motion_add_TEX->read_inds[0] = index_TEX;
	}
	static void read_BTYPE() {
		index_BTYPE = decoder_motion_add_BTYPE->read_inds[4];
		numTokens_BTYPE = index_BTYPE + fifo_i16_get_num_tokens(decoder_motion_add_BTYPE, 4);
	}
	
	static void read_end_BTYPE() {
		decoder_motion_add_BTYPE->read_inds[4] = index_BTYPE;
	}

	static void write_VID() {
		index_VID = (*decoder_motion_add_VID->write_ind);
		numFree_VID = index_VID + fifo_i16_get_room(decoder_motion_add_VID, NUM_READERS_VID);
	}
	
	static void write_end_VID() {
		(*decoder_motion_add_VID->write_ind) = index_VID;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures


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
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_cmd_textureOnly() {
		i32 result;
		i16 cmd;
		i32 local_INTRA;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_INTRA = ConstantsBtype_INTRA;
		result = (cmd & local_INTRA) != 0;
		return result;
	}
	
	static void cmd_textureOnly() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_cmd_motionOnly() {
		i32 result;
		i16 cmd;
		i32 local_ACCODED;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
		local_ACCODED = Constants_ACCODED;
		result = (cmd & local_ACCODED) == 0;
		return result;
	}
	
	static void cmd_motionOnly() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_cmd_other() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void cmd_other() {
	
		// Compute aligned port indexes
		i32 index_aligned_BTYPE = index_BTYPE % SIZE_BTYPE;
	
		i16 cmd;
	
		cmd = tokens_BTYPE[(index_BTYPE + (0)) % SIZE_BTYPE];
	
		// Update ports indexes
		index_BTYPE += 1;
		
		read_end_BTYPE();
	}
	static i32 isSchedulable_texture() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void texture() {
	
		// Compute aligned port indexes
		i32 index_aligned_TEX = index_TEX % SIZE_TEX;
		i32 index_aligned_VID = index_VID % SIZE_VID;
	
		i32 idx_TEX;
		i16 local_TEX;
	
		idx_TEX = 0;
		while (idx_TEX < 64) {
			local_TEX = tokens_TEX[(index_TEX + (idx_TEX)) % SIZE_TEX];
			tokens_VID[(index_VID + (idx_TEX)) % SIZE_VID] = local_TEX;
			idx_TEX = idx_TEX + 1;
		}
	
		// Update ports indexes
		index_TEX += 64;
		index_VID += 64;
		
		read_end_TEX();
		write_end_VID();
	}
	static i32 isSchedulable_motion() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void motion() {
	
		// Compute aligned port indexes
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
		i32 index_aligned_VID = index_VID % SIZE_VID;
	
		i32 idx_MOT;
		i16 local_MOT;
	
		idx_MOT = 0;
		while (idx_MOT < 64) {
			local_MOT = tokens_MOT[(index_MOT + (idx_MOT)) % SIZE_MOT];
			tokens_VID[(index_VID + (idx_MOT)) % SIZE_VID] = local_MOT;
			idx_MOT = idx_MOT + 1;
		}
	
		// Update ports indexes
		index_MOT += 64;
		index_VID += 64;
		
		read_end_MOT();
		write_end_VID();
	}
	static i32 isSchedulable_combine() {
		i32 result;
	
		result = 1;
		return result;
	}
	
	static void combine() {
	
		// Compute aligned port indexes
		i32 index_aligned_MOT = index_MOT % SIZE_MOT;
		i32 index_aligned_TEX = index_TEX % SIZE_TEX;
		i32 index_aligned_VID = index_VID % SIZE_VID;
	
		i16 add;
		i32 i;
		i16 tmp_TEX;
		i16 tmp_MOT;
	
		i = 0;
		while (i <= 63) {
			tmp_TEX = tokens_TEX[(index_TEX + (i)) % SIZE_TEX];
			tmp_MOT = tokens_MOT[(index_MOT + (i)) % SIZE_MOT];
			add = tmp_TEX + tmp_MOT;
			if (add < 0) {
				tokens_VID[(index_VID + (i)) % SIZE_VID] = 0;
			} else {
				if (add > 255) {
					tokens_VID[(index_VID + (i)) % SIZE_VID] = 255;
				} else {
					tokens_VID[(index_VID + (i)) % SIZE_VID] = add;
				}
			}
			i = i + 1;
		}
	
		// Update ports indexes
		index_MOT += 64;
		index_TEX += 64;
		index_VID += 64;
		
		read_end_MOT();
		read_end_TEX();
		write_end_VID();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_motion_add_initialize() {
		int i = 0;
		/* Set initial state to current FSM state */
		_FSM_state = my_state_cmd;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	
	int decoder_motion_add_scheduler() {
		int i = 0;
		if(needInit){
			decoder_motion_add_initialize();
			needInit = 0;
		}
	
		read_MOT();
		read_TEX();
		read_BTYPE();
		write_VID();
			
	
		
		// jump to FSM state 
		switch (_FSM_state) {
		case my_state_cmd:
			goto l_cmd;
		case my_state_combine:
			goto l_combine;
		case my_state_motion:
			goto l_motion;
		case my_state_skiph:
			goto l_skiph;
		case my_state_skipw:
			goto l_skipw;
		case my_state_texture:
			goto l_texture;
		default:
			exit(1);
		}
	
		// FSM transitions
		l_cmd:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_newVop()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_newVop();
				i++;
				goto l_skipw;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_textureOnly()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_textureOnly();
				i++;
				goto l_texture;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_motionOnly()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_motionOnly();
				i++;
				goto l_motion;
			} else if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_other()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_cmd;
					goto finished;
				}
				cmd_other();
				i++;
				goto l_combine;
			} else {
				_FSM_state = my_state_cmd;
				goto finished;
			}
		l_combine:
			if (numTokens_MOT - index_MOT >= 64 && numTokens_TEX - index_TEX >= 64 && isSchedulable_combine()) {
				int stop = 0;
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[0]) {
					stop = 1;
				}
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[1]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_combine;
					goto finished;
				}
				combine();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_combine;
				goto finished;
			}
		l_motion:
			if (numTokens_MOT - index_MOT >= 64 && isSchedulable_motion()) {
				int stop = 0;
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[0]) {
					stop = 1;
				}
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[1]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_motion;
					goto finished;
				}
				motion();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_motion;
				goto finished;
			}
		l_skiph:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_other()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_skiph;
					goto finished;
				}
				cmd_other();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_skiph;
				goto finished;
			}
		l_skipw:
			if (numTokens_BTYPE - index_BTYPE >= 1 && isSchedulable_cmd_other()) {
				int stop = 0;
				if (stop != 0) {
					_FSM_state = my_state_skipw;
					goto finished;
				}
				cmd_other();
				i++;
				goto l_skiph;
			} else {
				_FSM_state = my_state_skipw;
				goto finished;
			}
		l_texture:
			if (numTokens_TEX - index_TEX >= 64 && isSchedulable_texture()) {
				int stop = 0;
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[0]) {
					stop = 1;
				}
				if (64 > SIZE_VID - index_VID + decoder_motion_add_VID->read_inds[1]) {
					stop = 1;
				}
				if (stop != 0) {
					_FSM_state = my_state_texture;
					goto finished;
				}
				texture();
				i++;
				goto l_cmd;
			} else {
				_FSM_state = my_state_texture;
				goto finished;
			}
	finished:
		
		read_end_MOT();
		read_end_TEX();
		read_end_BTYPE();
		
		write_end_VID();
		
		
		return i;
	}
	
