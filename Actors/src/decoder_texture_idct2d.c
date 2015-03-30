	// Source file is "L/Research/src/fr/irisa/mpeg4/part2/sp/texture/Algo_IDCT2D_ISOIEC_23002_1.cal"

	#include <stdio.h>
	#include <stdlib.h>
	#include "dataflow.h"
	#include "fifoAllocations.h"
	
	



	

	////////////////////////////////////////////////////////////////////////////////
	// Input Fifo control variables
	static unsigned int index_IN;
	static unsigned int numTokens_IN;
	#define SIZE_IN 512
	#define tokens_IN 	decoder_texture_idct2d_IN->contents
	
	
	static unsigned int index_SIGNED;
	static unsigned int numTokens_SIGNED;
	#define SIZE_SIGNED 512
	#define tokens_SIGNED 	decoder_texture_idct2d_SIGNED->contents
	
	

	

	////////////////////////////////////////////////////////////////////////////////
	// Output Fifo control variables
	static unsigned int index_OUT;
	static unsigned int numFree_OUT;
	#define NUM_READERS_OUT 1
	#define SIZE_OUT 512
	#define tokens_OUT decoder_texture_idct2d_OUT->contents



	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// State variables of the actor
	#define A 1024
	#define B 1138
	#define C 1730
	#define D 1609
	#define E 1264
	#define F 1922
	#define G 1788
	#define H 2923
	#define I 2718
	#define J 2528
	static const i32 scale[64] = {1024, 1138, 1730, 1609, 1024, 1609, 1730, 1138, 1138, 1264, 1922, 1788, 1138, 1788, 1922, 1264, 1730, 1922, 2923, 2718, 1730, 2718, 2923, 1922, 1609, 1788, 2718, 2528, 1609, 2528, 2718, 1788, 1024, 1138, 1730, 1609, 1024, 1609, 1730, 1138, 1609, 1788, 2718, 2528, 1609, 2528, 2718, 1788, 1730, 1922, 2923, 2718, 1730, 2718, 2923, 1922, 1138, 1264, 1922, 1788, 1138, 1788, 1922, 1264};
	
	static u8 needInit = 1;
	
	////////////////////////////////////////////////////////////////////////////////
	// Token functions
	static void read_IN() {
		index_IN = decoder_texture_idct2d_IN->read_inds[0];
		numTokens_IN = index_IN + fifo_i16_get_num_tokens(decoder_texture_idct2d_IN, 0);
	}
	
	static void read_end_IN() {
		decoder_texture_idct2d_IN->read_inds[0] = index_IN;
	}
	static void read_SIGNED() {
		index_SIGNED = decoder_texture_idct2d_SIGNED->read_inds[0];
		numTokens_SIGNED = index_SIGNED + fifo_i32_get_num_tokens(decoder_texture_idct2d_SIGNED, 0);
	}
	
	static void read_end_SIGNED() {
		decoder_texture_idct2d_SIGNED->read_inds[0] = index_SIGNED;
	}

	static void write_OUT() {
		index_OUT = (*decoder_texture_idct2d_OUT->write_ind);
		numFree_OUT = index_OUT + fifo_i16_get_room(decoder_texture_idct2d_OUT, NUM_READERS_OUT);
	}
	
	static void write_end_OUT() {
		(*decoder_texture_idct2d_OUT->write_ind) = index_OUT;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Functions/procedures
	static i32 pmul_1_1(i32 X);
	static i32 pmul_1_2(i32 X);
	static i32 pmul_2_1(i32 X);
	static i32 pmul_2_2(i32 X);
	static i32 pmul_3_1(i32 X);
	static i32 pmul_3_2(i32 X);
	static i32 clip(i32 x, i32 lim);
	static void idct1d(i32 X[64], i32 Y[64]);

	static i32 pmul_1_1(i32 X) {
		i32 Temp1;
	
		Temp1 = (X >> 3) - (X >> 7);
		return X - Temp1;
	}
	static i32 pmul_1_2(i32 X) {
		i32 Temp1;
		i32 Temp2;
	
		Temp1 = (X >> 3) - (X >> 7);
		Temp2 = Temp1 - (X >> 11);
		return Temp1 + (Temp2 >> 1);
	}
	static i32 pmul_2_1(i32 X) {
		i32 Temp1;
	
		Temp1 = (X >> 9) - X;
		return (Temp1 >> 2) - Temp1;
	}
	static i32 pmul_2_2(i32 X) {
	
		return X >> 1;
	}
	static i32 pmul_3_1(i32 X) {
		i32 Temp1;
		i32 Temp2;
	
		Temp1 = X + (X >> 5);
		Temp2 = Temp1 >> 2;
		return Temp2 + (X >> 4);
	}
	static i32 pmul_3_2(i32 X) {
		i32 Temp1;
		i32 Temp2;
	
		Temp1 = X + (X >> 5);
		Temp2 = Temp1 >> 2;
		return Temp1 - Temp2;
	}
	static i32 clip(i32 x, i32 lim) {
		i32 tmp_if;
	
		if (x < lim) {
			tmp_if = lim;
		} else {
			if (x > 255) {
				tmp_if = 255;
			} else {
				tmp_if = x;
			}
		}
		return tmp_if;
	}
	static void idct1d(i32 X[64], i32 Y[64]) {
		i32 x0;
		i32 x1;
		i32 x2;
		i32 x3;
		i32 x4;
		i32 x5;
		i32 x6;
		i32 x7;
		i32 xa;
		i32 xb;
		i32 i;
	
		i = 0;
		while (i <= 7) {
			x1 = X[8 * i + 1];
			x3 = X[8 * i + 3];
			x5 = X[8 * i + 5];
			x7 = X[8 * i + 7];
			xa = x1 + x7;
			xb = x1 - x7;
			x1 = xa + x3;
			x3 = xa - x3;
			x7 = xb + x5;
			x5 = xb - x5;
			xa = pmul_1_2(x3);
			x3 = pmul_1_1(x3);
			xb = pmul_1_2(x5);
			x5 = pmul_1_1(x5);
			x3 = x3 - xb;
			x5 = x5 + xa;
			xa = pmul_2_2(x1);
			x1 = pmul_2_1(x1);
			xb = pmul_2_2(x7);
			x7 = pmul_2_1(x7);
			x1 = x1 + xb;
			x7 = x7 - xa;
			x0 = X[8 * i + 0];
			x2 = X[8 * i + 2];
			x4 = X[8 * i + 4];
			x6 = X[8 * i + 6];
			xa = pmul_3_2(x2);
			x2 = pmul_3_1(x2);
			xb = pmul_3_2(x6);
			x6 = pmul_3_1(x6);
			x2 = x2 - xb;
			x6 = x6 + xa;
			xa = x0 + x4;
			xb = x0 - x4;
			x0 = xa + x6;
			x6 = xa - x6;
			x4 = xb + x2;
			x2 = xb - x2;
			Y[i + 8 * 0] = x0 + x1;
			Y[i + 8 * 1] = x4 + x5;
			Y[i + 8 * 2] = x2 + x3;
			Y[i + 8 * 3] = x6 + x7;
			Y[i + 8 * 4] = x6 - x7;
			Y[i + 8 * 5] = x2 - x3;
			Y[i + 8 * 6] = x4 - x5;
			Y[i + 8 * 7] = x0 - x1;
			i = i + 1;
		}
	}

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
		i32 index_aligned_SIGNED = index_SIGNED % SIZE_SIGNED;
		i32 index_aligned_OUT = index_OUT % SIZE_OUT;
	
		i32 s;
		i32 n;
		i32 tmp_scale;
		i16 tmp_IN;
		i32 block1[64];
		i32 block2[64];
		i32 tmp_block1;
		i32 tmp_block10;
		i32 tmp_clip;
		i32 tmp_block11;
		i32 tmp_clip0;
	
		s = tokens_SIGNED[(index_SIGNED + (0)) % SIZE_SIGNED];
		n = 0;
		while (n <= 63) {
			tmp_scale = scale[n];
			tmp_IN = tokens_IN[(index_IN + (n)) % SIZE_IN];
			block1[n] = tmp_scale * tmp_IN;
			n = n + 1;
		}
		tmp_block1 = block1[0];
		block1[0] = tmp_block1 + (1 << 12);
		idct1d(block1, block2);
		idct1d(block2, block1);
		if (s) {
			n = 0;
			while (n <= 63) {
				tmp_block10 = block1[n];
				tmp_clip = clip(tmp_block10 >> 13, -255);
				tokens_OUT[(index_OUT + (n)) % SIZE_OUT] = tmp_clip;
				n = n + 1;
			}
		} else {
			n = 0;
			while (n <= 63) {
				tmp_block11 = block1[n];
				tmp_clip0 = clip(tmp_block11 >> 13, 0);
				tokens_OUT[(index_OUT + (n)) % SIZE_OUT] = tmp_clip0;
				n = n + 1;
			}
		}
	
		// Update ports indexes
		index_IN += 64;
		index_SIGNED += 1;
		index_OUT += 64;
		
		read_end_IN();
		read_end_SIGNED();
		write_end_OUT();
	}

	
	////////////////////////////////////////////////////////////////////////////////
	// Initializes
	
	void decoder_texture_idct2d_initialize() {
		int i = 0;
		
	finished:
		// no read_end/write_end here!
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Action scheduler
	int decoder_texture_idct2d_scheduler() {
		int i = 0;
		if(needInit){
			decoder_texture_idct2d_initialize();
			needInit = 0;
		}
		read_IN();
		read_SIGNED();
		write_OUT();
		
	
	
		while (1) {
			// Action loop
			if (numTokens_IN - index_IN >= 64 && numTokens_SIGNED - index_SIGNED >= 1 && isSchedulable_untagged_0()) {
				int stop = 0;
				if (64 > SIZE_OUT - index_OUT + decoder_texture_idct2d_OUT->read_inds[0]) {
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
		read_end_SIGNED();
		write_end_OUT();
		
		
		return i;
	}
	
