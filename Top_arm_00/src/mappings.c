/*
 * mappings.c
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */

#include "xparameters_ps.h"
#include "xuartps_hw.h"
#include "actors.h"
#include "mappings.h"
#include "ctrlComms.h"



static boolean mapping [NB_PROCESSORS][NB_PROCESSORS][NB_ACTORS];

static void setMapping_00() { // Mapping index 0, 1 processors

	// Processor index 0
	mapping[0][0][decoder_merger] = true;
	mapping[0][0][decoder_parser_parseheaders] = true;
	mapping[0][0][decoder_parser_mvseq] = true;
	mapping[0][0][decoder_parser_blkexp] = true;
	mapping[0][0][decoder_parser_mvrecon] = true;
	mapping[0][0][decoder_texture_DCsplit] = true;
	mapping[0][0][decoder_texture_IS] = true;
	mapping[0][0][decoder_texture_IAP] = true;
	mapping[0][0][decoder_texture_IQ] = true;
	mapping[0][0][decoder_texture_idct2d] = true;
	mapping[0][0][decoder_texture_DCReconstruction_addressing] = true;
	mapping[0][0][decoder_texture_DCReconstruction_invpred] = true;
	mapping[0][0][decoder_motion_interpolation] = true;
	mapping[0][0][decoder_motion_add] = true;
	mapping[0][0][decoder_motion_framebuf] = true;
}

static void setMapping_01() { // Mapping index 1, 2 processors

	// Processor index 0
	mapping[1][0][decoder_merger] = true;
	mapping[1][0][decoder_parser_parseheaders] = true;
	mapping[1][0][decoder_parser_mvseq] = true;
	mapping[1][0][decoder_parser_blkexp] = false;
	mapping[1][0][decoder_parser_mvrecon] = false;
	mapping[1][0][decoder_texture_DCsplit] = false;
	mapping[1][0][decoder_texture_IS] = false;
	mapping[1][0][decoder_texture_IAP] = false;
	mapping[1][0][decoder_texture_IQ] = false;
	mapping[1][0][decoder_texture_idct2d] = false;
	mapping[1][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[1][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[1][0][decoder_motion_interpolation] = false;
	mapping[1][0][decoder_motion_add] = false;
	mapping[1][0][decoder_motion_framebuf] = false;

	// Mapping index 1, Processor index 1
	mapping[1][1][decoder_merger] = false;
	mapping[1][1][decoder_parser_parseheaders] = false;
	mapping[1][1][decoder_parser_mvseq] = false;
	mapping[1][1][decoder_parser_blkexp] = true;
	mapping[1][1][decoder_parser_mvrecon] = true;
	mapping[1][1][decoder_texture_DCsplit] = true;
	mapping[1][1][decoder_texture_IS] = true;
	mapping[1][1][decoder_texture_IAP] = true;
	mapping[1][1][decoder_texture_IQ] = true;
	mapping[1][1][decoder_texture_idct2d] = true;
	mapping[1][1][decoder_texture_DCReconstruction_addressing] = true;
	mapping[1][1][decoder_texture_DCReconstruction_invpred] = true;
	mapping[1][1][decoder_motion_interpolation] = true;
	mapping[1][1][decoder_motion_add] = true;
	mapping[1][1][decoder_motion_framebuf] = true;
}


static void setMapping_02() { // Mapping index 2, 3 processors

	// Processor index 0
	mapping[2][0][decoder_merger] = false;
	mapping[2][0][decoder_parser_parseheaders] = false;
	mapping[2][0][decoder_parser_mvseq] = false;
	mapping[2][0][decoder_parser_blkexp] = false;
	mapping[2][0][decoder_parser_mvrecon] = false;
	mapping[2][0][decoder_texture_DCsplit] = false;
	mapping[2][0][decoder_texture_IS] = false;
	mapping[2][0][decoder_texture_IAP] = false;
	mapping[2][0][decoder_texture_IQ] = false;
	mapping[2][0][decoder_texture_idct2d] = false;
	mapping[2][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[2][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[2][0][decoder_motion_interpolation] = false;
	mapping[2][0][decoder_motion_add] = false;
	mapping[2][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[2][1][decoder_merger] = true;
	mapping[2][1][decoder_parser_parseheaders] = true;
	mapping[2][1][decoder_parser_mvseq] = true;
	mapping[2][1][decoder_parser_blkexp] = true;
	mapping[2][1][decoder_parser_mvrecon] = false;
	mapping[2][1][decoder_texture_DCsplit] = true;
	mapping[2][1][decoder_texture_IS] = true;
	mapping[2][1][decoder_texture_IAP] = true;
	mapping[2][1][decoder_texture_IQ] = false;
	mapping[2][1][decoder_texture_idct2d] = true;
	mapping[2][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[2][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[2][1][decoder_motion_interpolation] = false;
	mapping[2][1][decoder_motion_add] = false;
	mapping[2][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[2][2][decoder_merger] = false;
	mapping[2][2][decoder_parser_parseheaders] = false;
	mapping[2][2][decoder_parser_mvseq] = false;
	mapping[2][2][decoder_parser_blkexp] = false;
	mapping[2][2][decoder_parser_mvrecon] = true;
	mapping[2][2][decoder_texture_DCsplit] = false;
	mapping[2][2][decoder_texture_IS] = false;
	mapping[2][2][decoder_texture_IAP] = false;
	mapping[2][2][decoder_texture_IQ] = true;
	mapping[2][2][decoder_texture_idct2d] = false;
	mapping[2][2][decoder_texture_DCReconstruction_addressing] = true;
	mapping[2][2][decoder_texture_DCReconstruction_invpred] = true;
	mapping[2][2][decoder_motion_interpolation] = true;
	mapping[2][2][decoder_motion_add] = true;
	mapping[2][2][decoder_motion_framebuf] = true;
}
//
//		static void setMapping_02(){	// Mapping index 2, 3 processors
//
//			// Processor index 0
//			mapping[2][0][source] = true;
//			mapping[2][0][display] = true;
//			mapping[2][0][decoder_merger] = true;
//			mapping[2][0][decoder_parser_parseheaders] = false;
//			mapping[2][0][decoder_parser_mvseq] = true;
//			mapping[2][0][decoder_parser_blkexp] = true;
//			mapping[2][0][decoder_parser_mvrecon] = true;
//			mapping[2][0][decoder_texture_DCsplit] = true;
//			mapping[2][0][decoder_texture_IS] = true;
//			mapping[2][0][decoder_texture_IAP] = true;
//			mapping[2][0][decoder_texture_IQ] = false;
//			mapping[2][0][decoder_texture_idct2d] = false;
//			mapping[2][0][decoder_texture_DCReconstruction_addressing] = false;
//			mapping[2][0][decoder_texture_DCReconstruction_invpred] = false;
//			mapping[2][0][decoder_motion_interpolation] = false;
//			mapping[2][0][decoder_motion_add] = false;
//			mapping[2][0][decoder_motion_framebuf] = false;
//
//			// Processor index 1
//			mapping[2][1][source] = false;
//			mapping[2][1][display] = false;
//			mapping[2][1][decoder_merger] = false;
//			mapping[2][1][decoder_parser_parseheaders] = false;
//			mapping[2][1][decoder_parser_mvseq] = false;
//			mapping[2][1][decoder_parser_blkexp] = false;
//			mapping[2][1][decoder_parser_mvrecon] = false;
//			mapping[2][1][decoder_texture_DCsplit] = false;
//			mapping[2][1][decoder_texture_IS] = false;
//			mapping[2][1][decoder_texture_IAP] = false;
//			mapping[2][1][decoder_texture_IQ] = true;
//			mapping[2][1][decoder_texture_idct2d] = true;
//			mapping[2][1][decoder_texture_DCReconstruction_addressing] = true;
//			mapping[2][1][decoder_texture_DCReconstruction_invpred] = true;
//			mapping[2][1][decoder_motion_interpolation] = true;
//			mapping[2][1][decoder_motion_add] = false;
//			mapping[2][1][decoder_motion_framebuf] = false;
//
//			// Processor index 2
//			mapping[2][2][source] = false;
//			mapping[2][2][display] = false;
//			mapping[2][2][decoder_merger] = false;
//			mapping[2][2][decoder_parser_parseheaders] = true;
//			mapping[2][2][decoder_parser_mvseq] = false;
//			mapping[2][2][decoder_parser_blkexp] = false;
//			mapping[2][2][decoder_parser_mvrecon] = false;
//			mapping[2][2][decoder_texture_DCsplit] = false;
//			mapping[2][2][decoder_texture_IS] = false;
//			mapping[2][2][decoder_texture_IAP] = false;
//			mapping[2][2][decoder_texture_IQ] = false;
//			mapping[2][2][decoder_texture_idct2d] = false;
//			mapping[2][2][decoder_texture_DCReconstruction_addressing] = false;
//			mapping[2][2][decoder_texture_DCReconstruction_invpred] = false;
//			mapping[2][2][decoder_motion_interpolation] = false;
//			mapping[2][2][decoder_motion_add] = true;
//			mapping[2][2][decoder_motion_framebuf] = true;
//		}

static void setMapping_03(){	// Mapping index 3, 4 processors

	// Processor index 0
	mapping[3][0][decoder_merger] = false;
	mapping[3][0][decoder_parser_parseheaders] = false;
	mapping[3][0][decoder_parser_mvseq] = false;
	mapping[3][0][decoder_parser_blkexp] = false;
	mapping[3][0][decoder_parser_mvrecon] = false;
	mapping[3][0][decoder_texture_DCsplit] = false;
	mapping[3][0][decoder_texture_IS] = false;
	mapping[3][0][decoder_texture_IAP] = false;
	mapping[3][0][decoder_texture_IQ] = false;
	mapping[3][0][decoder_texture_idct2d] = false;
	mapping[3][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[3][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[3][0][decoder_motion_interpolation] = false;
	mapping[3][0][decoder_motion_add] = false;
	mapping[3][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[3][1][decoder_merger] = true;
	mapping[3][1][decoder_parser_parseheaders] = true;
	mapping[3][1][decoder_parser_mvseq] = true;
	mapping[3][1][decoder_parser_blkexp] = true;
	mapping[3][1][decoder_parser_mvrecon] = true;
	mapping[3][1][decoder_texture_DCsplit] = false;
	mapping[3][1][decoder_texture_IS] = false;
	mapping[3][1][decoder_texture_IAP] = false;
	mapping[3][1][decoder_texture_IQ] = false;
	mapping[3][1][decoder_texture_idct2d] = false;
	mapping[3][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[3][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[3][1][decoder_motion_interpolation] = false;
	mapping[3][1][decoder_motion_add] = false;
	mapping[3][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[3][2][decoder_merger] = false;
	mapping[3][2][decoder_parser_parseheaders] = false;
	mapping[3][2][decoder_parser_mvseq] = false;
	mapping[3][2][decoder_parser_blkexp] = false;
	mapping[3][2][decoder_parser_mvrecon] = false;
	mapping[3][2][decoder_texture_DCsplit] = true;
	mapping[3][2][decoder_texture_IS] = true;
	mapping[3][2][decoder_texture_IAP] = true;
	mapping[3][2][decoder_texture_IQ] = true;
	mapping[3][2][decoder_texture_idct2d] = true;
	mapping[3][2][decoder_texture_DCReconstruction_addressing] = true;
	mapping[3][2][decoder_texture_DCReconstruction_invpred] = true;
	mapping[3][2][decoder_motion_interpolation] = false;
	mapping[3][2][decoder_motion_add] = false;
	mapping[3][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[3][3][decoder_merger] = false;
	mapping[3][3][decoder_parser_parseheaders] = false;
	mapping[3][3][decoder_parser_mvseq] = false;
	mapping[3][3][decoder_parser_blkexp] = false;
	mapping[3][3][decoder_parser_mvrecon] = false;
	mapping[3][3][decoder_texture_DCsplit] = false;
	mapping[3][3][decoder_texture_IS] = false;
	mapping[3][3][decoder_texture_IAP] = false;
	mapping[3][3][decoder_texture_IQ] = false;
	mapping[3][3][decoder_texture_idct2d] = false;
	mapping[3][3][decoder_texture_DCReconstruction_addressing] = false;
	mapping[3][3][decoder_texture_DCReconstruction_invpred] = false;
	mapping[3][3][decoder_motion_interpolation] = true;
	mapping[3][3][decoder_motion_add] = true;
	mapping[3][3][decoder_motion_framebuf] = true;
}
//
//		static void setMapping_03(){	// Mapping index 3, 4 processors
//
//			// Processor index 0
//			mapping[3][0][source] = true;
//			mapping[3][0][display] = true;
//			mapping[3][0][decoder_merger] = true;
//			mapping[3][0][decoder_parser_parseheaders] = true;
//			mapping[3][0][decoder_parser_mvseq] = true;
//			mapping[3][0][decoder_parser_blkexp] = true;
//			mapping[3][0][decoder_parser_mvrecon] = true;
//			mapping[3][0][decoder_texture_DCsplit] = false;
//			mapping[3][0][decoder_texture_IS] = false;
//			mapping[3][0][decoder_texture_IAP] = false;
//			mapping[3][0][decoder_texture_IQ] = false;
//			mapping[3][0][decoder_texture_idct2d] = false;
//			mapping[3][0][decoder_texture_DCReconstruction_addressing] = false;
//			mapping[3][0][decoder_texture_DCReconstruction_invpred] = false;
//			mapping[3][0][decoder_motion_interpolation] = false;
//			mapping[3][0][decoder_motion_add] = false;
//			mapping[3][0][decoder_motion_framebuf] = false;
//
//			// Processor index 1
//			mapping[3][1][source] = false;
//			mapping[3][1][display] = false;
//			mapping[3][1][decoder_merger] = false;
//			mapping[3][1][decoder_parser_parseheaders] = false;
//			mapping[3][1][decoder_parser_mvseq] = false;
//			mapping[3][1][decoder_parser_blkexp] = false;
//			mapping[3][1][decoder_parser_mvrecon] = false;
//			mapping[3][1][decoder_texture_DCsplit] = true;
//			mapping[3][1][decoder_texture_IS] = true;
//			mapping[3][1][decoder_texture_IAP] = true;
//			mapping[3][1][decoder_texture_IQ] = true;
//			mapping[3][1][decoder_texture_idct2d] = false;
//			mapping[3][1][decoder_texture_DCReconstruction_addressing] = false;
//			mapping[3][1][decoder_texture_DCReconstruction_invpred] = false;
//			mapping[3][1][decoder_motion_interpolation] = false;
//			mapping[3][1][decoder_motion_add] = false;
//			mapping[3][1][decoder_motion_framebuf] = false;
//
//			// Processor index 2
//			mapping[3][2][source] = false;
//			mapping[3][2][display] = false;
//			mapping[3][2][decoder_merger] = false;
//			mapping[3][2][decoder_parser_parseheaders] = false;
//			mapping[3][2][decoder_parser_mvseq] = false;
//			mapping[3][2][decoder_parser_blkexp] = false;
//			mapping[3][2][decoder_parser_mvrecon] = false;
//			mapping[3][2][decoder_texture_DCsplit] = false;
//			mapping[3][2][decoder_texture_IS] = false;
//			mapping[3][2][decoder_texture_IAP] = false;
//			mapping[3][2][decoder_texture_IQ] = false;
//			mapping[3][2][decoder_texture_idct2d] = true;
//			mapping[3][2][decoder_texture_DCReconstruction_addressing] = true;
//			mapping[3][2][decoder_texture_DCReconstruction_invpred] = true;
//			mapping[3][2][decoder_motion_interpolation] = true;
//			mapping[3][2][decoder_motion_add] = false;
//			mapping[3][2][decoder_motion_framebuf] = false;
//
//			// Processor index 3
//			mapping[3][3][source] = false;
//			mapping[3][3][display] = false;
//			mapping[3][3][decoder_merger] = false;
//			mapping[3][3][decoder_parser_parseheaders] = false;
//			mapping[3][3][decoder_parser_mvseq] = false;
//			mapping[3][3][decoder_parser_blkexp] = false;
//			mapping[3][3][decoder_parser_mvrecon] = false;
//			mapping[3][3][decoder_texture_DCsplit] = false;
//			mapping[3][3][decoder_texture_IS] = false;
//			mapping[3][3][decoder_texture_IAP] = false;
//			mapping[3][3][decoder_texture_IQ] = false;
//			mapping[3][3][decoder_texture_idct2d] = false;
//			mapping[3][3][decoder_texture_DCReconstruction_addressing] = false;
//			mapping[3][3][decoder_texture_DCReconstruction_invpred] = false;
//			mapping[3][3][decoder_motion_interpolation] = false;
//			mapping[3][3][decoder_motion_add] = true;
//			mapping[3][3][decoder_motion_framebuf] = true;
//		}

static void setMapping_04(){	// Mapping index 4, 5 processors

	// Processor index 0
	mapping[4][0][decoder_merger] = true;
	mapping[4][0][decoder_parser_parseheaders] = false;
	mapping[4][0][decoder_parser_mvseq] = false;
	mapping[4][0][decoder_parser_blkexp] = false;
	mapping[4][0][decoder_parser_mvrecon] = false;
	mapping[4][0][decoder_texture_DCsplit] = false;
	mapping[4][0][decoder_texture_IS] = false;
	mapping[4][0][decoder_texture_IAP] = false;
	mapping[4][0][decoder_texture_IQ] = false;
	mapping[4][0][decoder_texture_idct2d] = false;
	mapping[4][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[4][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[4][0][decoder_motion_interpolation] = false;
	mapping[4][0][decoder_motion_add] = false;
	mapping[4][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[4][1][decoder_merger] = false;
	mapping[4][1][decoder_parser_parseheaders] = true;
	mapping[4][1][decoder_parser_mvseq] = true;
	mapping[4][1][decoder_parser_blkexp] = true;
	mapping[4][1][decoder_parser_mvrecon] = true;
	mapping[4][1][decoder_texture_DCsplit] = false;
	mapping[4][1][decoder_texture_IS] = false;
	mapping[4][1][decoder_texture_IAP] = false;
	mapping[4][1][decoder_texture_IQ] = false;
	mapping[4][1][decoder_texture_idct2d] = false;
	mapping[4][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[4][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[4][1][decoder_motion_interpolation] = false;
	mapping[4][1][decoder_motion_add] = false;
	mapping[4][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[4][2][decoder_merger] = false;
	mapping[4][2][decoder_parser_parseheaders] = false;
	mapping[4][2][decoder_parser_mvseq] = false;
	mapping[4][2][decoder_parser_blkexp] = false;
	mapping[4][2][decoder_parser_mvrecon] = false;
	mapping[4][2][decoder_texture_DCsplit] = true;
	mapping[4][2][decoder_texture_IS] = true;
	mapping[4][2][decoder_texture_IAP] = true;
	mapping[4][2][decoder_texture_IQ] = true;
	mapping[4][2][decoder_texture_idct2d] = true;
	mapping[4][2][decoder_texture_DCReconstruction_addressing] = true;
	mapping[4][2][decoder_texture_DCReconstruction_invpred] = true;
	mapping[4][2][decoder_motion_interpolation] = false;
	mapping[4][2][decoder_motion_add] = false;
	mapping[4][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[4][3][decoder_merger] = false;
	mapping[4][3][decoder_parser_parseheaders] = false;
	mapping[4][3][decoder_parser_mvseq] = false;
	mapping[4][3][decoder_parser_blkexp] = false;
	mapping[4][3][decoder_parser_mvrecon] = false;
	mapping[4][3][decoder_texture_DCsplit] = false;
	mapping[4][3][decoder_texture_IS] = false;
	mapping[4][3][decoder_texture_IAP] = false;
	mapping[4][3][decoder_texture_IQ] = false;
	mapping[4][3][decoder_texture_idct2d] = false;
	mapping[4][3][decoder_texture_DCReconstruction_addressing] = false;
	mapping[4][3][decoder_texture_DCReconstruction_invpred] = false;
	mapping[4][3][decoder_motion_interpolation] = true;
	mapping[4][3][decoder_motion_add] = true;
	mapping[4][3][decoder_motion_framebuf] = false;

	// Processor index 4
	mapping[4][4][decoder_merger] = false;
	mapping[4][4][decoder_parser_parseheaders] = false;
	mapping[4][4][decoder_parser_mvseq] = false;
	mapping[4][4][decoder_parser_blkexp] = false;
	mapping[4][4][decoder_parser_mvrecon] = false;
	mapping[4][4][decoder_texture_DCsplit] = false;
	mapping[4][4][decoder_texture_IS] = false;
	mapping[4][4][decoder_texture_IAP] = false;
	mapping[4][4][decoder_texture_IQ] = false;
	mapping[4][4][decoder_texture_idct2d] = false;
	mapping[4][4][decoder_texture_DCReconstruction_addressing] = false;
	mapping[4][4][decoder_texture_DCReconstruction_invpred] = false;
	mapping[4][4][decoder_motion_interpolation] = false;
	mapping[4][4][decoder_motion_add] = false;
	mapping[4][4][decoder_motion_framebuf] = true;
}
//
//static void setMapping_05(){	// Mapping index 5, 6 processors
//
//	// Processor index 0
//	mapping[5][0][source] = false;
//	mapping[5][0][display] = false;
//	mapping[5][0][decoder_merger] = true;
//	mapping[5][0][decoder_parser_parseheaders] = true;
//	mapping[5][0][decoder_parser_mvseq] = false;
//	mapping[5][0][decoder_parser_blkexp] = false;
//	mapping[5][0][decoder_parser_mvrecon] = false;
//	mapping[5][0][decoder_texture_DCsplit] = false;
//	mapping[5][0][decoder_texture_IS] = false;
//	mapping[5][0][decoder_texture_IAP] = false;
//	mapping[5][0][decoder_texture_IQ] = false;
//	mapping[5][0][decoder_texture_idct2d] = false;
//	mapping[5][0][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[5][0][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[5][0][decoder_motion_interpolation] = false;
//	mapping[5][0][decoder_motion_add] = false;
//	mapping[5][0][decoder_motion_framebuf] = false;
//
//	// Processor index 1
//	mapping[5][1][source] = false;
//	mapping[5][1][display] = false;
//	mapping[5][1][decoder_merger] = false;
//	mapping[5][1][decoder_parser_parseheaders] = false;
//	mapping[5][1][decoder_parser_mvseq] = true;
//	mapping[5][1][decoder_parser_blkexp] = true;
//	mapping[5][1][decoder_parser_mvrecon] = true;
//	mapping[5][1][decoder_texture_DCsplit] = true;
//	mapping[5][1][decoder_texture_IS] = true;
//	mapping[5][1][decoder_texture_IAP] = true;
//	mapping[5][1][decoder_texture_IQ] = false;
//	mapping[5][1][decoder_texture_idct2d] = false;
//	mapping[5][1][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[5][1][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[5][1][decoder_motion_interpolation] = false;
//	mapping[5][1][decoder_motion_add] = false;
//	mapping[5][1][decoder_motion_framebuf] = false;
//
//	// Processor index 2
//	mapping[5][2][source] = false;
//	mapping[5][2][display] = false;
//	mapping[5][2][decoder_merger] = false;
//	mapping[5][2][decoder_parser_parseheaders] = false;
//	mapping[5][2][decoder_parser_mvseq] = false;
//	mapping[5][2][decoder_parser_blkexp] = false;
//	mapping[5][2][decoder_parser_mvrecon] = false;
//	mapping[5][2][decoder_texture_DCsplit] = false;
//	mapping[5][2][decoder_texture_IS] = false;
//	mapping[5][2][decoder_texture_IAP] = false;
//	mapping[5][2][decoder_texture_IQ] = true;
//	mapping[5][2][decoder_texture_idct2d] = true;
//	mapping[5][2][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[5][2][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[5][2][decoder_motion_interpolation] = false;
//	mapping[5][2][decoder_motion_add] = false;
//	mapping[5][2][decoder_motion_framebuf] = false;
//
//	// Processor index 3
//	mapping[5][3][source] = false;
//	mapping[5][3][display] = false;
//	mapping[5][3][decoder_merger] = false;
//	mapping[5][3][decoder_parser_parseheaders] = false;
//	mapping[5][3][decoder_parser_mvseq] = false;
//	mapping[5][3][decoder_parser_blkexp] = false;
//	mapping[5][3][decoder_parser_mvrecon] = false;
//	mapping[5][3][decoder_texture_DCsplit] = false;
//	mapping[5][3][decoder_texture_IS] = false;
//	mapping[5][3][decoder_texture_IAP] = false;
//	mapping[5][3][decoder_texture_IQ] = false;
//	mapping[5][3][decoder_texture_idct2d] = false;
//	mapping[5][3][decoder_texture_DCReconstruction_addressing] = true;
//	mapping[5][3][decoder_texture_DCReconstruction_invpred] = true;
//	mapping[5][3][decoder_motion_interpolation] = true;
//	mapping[5][3][decoder_motion_add] = false;
//	mapping[5][3][decoder_motion_framebuf] = false;
//
//	// Processor index 4
//	mapping[5][4][source] = false;
//	mapping[5][4][display] = false;
//	mapping[5][4][decoder_merger] = false;
//	mapping[5][4][decoder_parser_parseheaders] = false;
//	mapping[5][4][decoder_parser_mvseq] = false;
//	mapping[5][4][decoder_parser_blkexp] = false;
//	mapping[5][4][decoder_parser_mvrecon] = false;
//	mapping[5][4][decoder_texture_DCsplit] = false;
//	mapping[5][4][decoder_texture_IS] = false;
//	mapping[5][4][decoder_texture_IAP] = false;
//	mapping[5][4][decoder_texture_IQ] = false;
//	mapping[5][4][decoder_texture_idct2d] = false;
//	mapping[5][4][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[5][4][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[5][4][decoder_motion_interpolation] = false;
//	mapping[5][4][decoder_motion_add] = true;
//	mapping[5][4][decoder_motion_framebuf] = false;
//
//	// Processor index 5
//	mapping[5][5][source] = false;
//	mapping[5][5][display] = false;
//	mapping[5][5][decoder_merger] = false;
//	mapping[5][5][decoder_parser_parseheaders] = false;
//	mapping[5][5][decoder_parser_mvseq] = false;
//	mapping[5][5][decoder_parser_blkexp] = false;
//	mapping[5][5][decoder_parser_mvrecon] = false;
//	mapping[5][5][decoder_texture_DCsplit] = false;
//	mapping[5][5][decoder_texture_IS] = false;
//	mapping[5][5][decoder_texture_IAP] = false;
//	mapping[5][5][decoder_texture_IQ] = false;
//	mapping[5][5][decoder_texture_idct2d] = false;
//	mapping[5][5][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[5][5][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[5][5][decoder_motion_interpolation] = false;
//	mapping[5][5][decoder_motion_add] = false;
//	mapping[5][5][decoder_motion_framebuf] = true;
//}
//
//static void setMapping_06(){	// Mapping index 6, 7 processors
//
//	// Processor index 0
//	mapping[6][0][source] = false;
//	mapping[6][0][display] = false;
//	mapping[6][0][decoder_merger] = true;
//	mapping[6][0][decoder_parser_parseheaders] = true;
//	mapping[6][0][decoder_parser_mvseq] = false;
//	mapping[6][0][decoder_parser_blkexp] = false;
//	mapping[6][0][decoder_parser_mvrecon] = false;
//	mapping[6][0][decoder_texture_DCsplit] = false;
//	mapping[6][0][decoder_texture_IS] = false;
//	mapping[6][0][decoder_texture_IAP] = false;
//	mapping[6][0][decoder_texture_IQ] = false;
//	mapping[6][0][decoder_texture_idct2d] = false;
//	mapping[6][0][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][0][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][0][decoder_motion_interpolation] = false;
//	mapping[6][0][decoder_motion_add] = false;
//	mapping[6][0][decoder_motion_framebuf] = false;
//
//	// Processor index 1
//	mapping[6][1][source] = false;
//	mapping[6][1][display] = false;
//	mapping[6][1][decoder_merger] = false;
//	mapping[6][1][decoder_parser_parseheaders] = false;
//	mapping[6][1][decoder_parser_mvseq] = true;
//	mapping[6][1][decoder_parser_blkexp] = true;
//	mapping[6][1][decoder_parser_mvrecon] = true;
//	mapping[6][1][decoder_texture_DCsplit] = true;
//	mapping[6][1][decoder_texture_IS] = true;
//	mapping[6][1][decoder_texture_IAP] = false;
//	mapping[6][1][decoder_texture_IQ] = false;
//	mapping[6][1][decoder_texture_idct2d] = false;
//	mapping[6][1][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][1][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][1][decoder_motion_interpolation] = false;
//	mapping[6][1][decoder_motion_add] = false;
//	mapping[6][1][decoder_motion_framebuf] = false;
//
//	// Processor index 2
//	mapping[6][2][source] = false;
//	mapping[6][2][display] = false;
//	mapping[6][2][decoder_merger] = false;
//	mapping[6][2][decoder_parser_parseheaders] = false;
//	mapping[6][2][decoder_parser_mvseq] = false;
//	mapping[6][2][decoder_parser_blkexp] = false;
//	mapping[6][2][decoder_parser_mvrecon] = false;
//	mapping[6][2][decoder_texture_DCsplit] = false;
//	mapping[6][2][decoder_texture_IS] = false;
//	mapping[6][2][decoder_texture_IAP] = true;
//	mapping[6][2][decoder_texture_IQ] = true;
//	mapping[6][2][decoder_texture_idct2d] = false;
//	mapping[6][2][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][2][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][2][decoder_motion_interpolation] = false;
//	mapping[6][2][decoder_motion_add] = false;
//	mapping[6][2][decoder_motion_framebuf] = false;
//
//	// Processor index 3
//	mapping[6][3][source] = false;
//	mapping[6][3][display] = false;
//	mapping[6][3][decoder_merger] = false;
//	mapping[6][3][decoder_parser_parseheaders] = false;
//	mapping[6][3][decoder_parser_mvseq] = false;
//	mapping[6][3][decoder_parser_blkexp] = false;
//	mapping[6][3][decoder_parser_mvrecon] = false;
//	mapping[6][3][decoder_texture_DCsplit] = false;
//	mapping[6][3][decoder_texture_IS] = false;
//	mapping[6][3][decoder_texture_IAP] = false;
//	mapping[6][3][decoder_texture_IQ] = false;
//	mapping[6][3][decoder_texture_idct2d] = true;
//	mapping[6][3][decoder_texture_DCReconstruction_addressing] = true;
//	mapping[6][3][decoder_texture_DCReconstruction_invpred] = true;
//	mapping[6][3][decoder_motion_interpolation] = false;
//	mapping[6][3][decoder_motion_add] = false;
//	mapping[6][3][decoder_motion_framebuf] = false;
//
//	// Processor index 4
//	mapping[6][4][source] = false;
//	mapping[6][4][display] = false;
//	mapping[6][4][decoder_merger] = false;
//	mapping[6][4][decoder_parser_parseheaders] = false;
//	mapping[6][4][decoder_parser_mvseq] = false;
//	mapping[6][4][decoder_parser_blkexp] = false;
//	mapping[6][4][decoder_parser_mvrecon] = false;
//	mapping[6][4][decoder_texture_DCsplit] = false;
//	mapping[6][4][decoder_texture_IS] = false;
//	mapping[6][4][decoder_texture_IAP] = false;
//	mapping[6][4][decoder_texture_IQ] = false;
//	mapping[6][4][decoder_texture_idct2d] = false;
//	mapping[6][4][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][4][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][4][decoder_motion_interpolation] = true;
//	mapping[6][4][decoder_motion_add] = false;
//	mapping[6][4][decoder_motion_framebuf] = false;
//
//	// Processor index 5
//	mapping[6][5][source] = false;
//	mapping[6][5][display] = false;
//	mapping[6][5][decoder_merger] = false;
//	mapping[6][5][decoder_parser_parseheaders] = false;
//	mapping[6][5][decoder_parser_mvseq] = false;
//	mapping[6][5][decoder_parser_blkexp] = false;
//	mapping[6][5][decoder_parser_mvrecon] = false;
//	mapping[6][5][decoder_texture_DCsplit] = false;
//	mapping[6][5][decoder_texture_IS] = false;
//	mapping[6][5][decoder_texture_IAP] = false;
//	mapping[6][5][decoder_texture_IQ] = false;
//	mapping[6][5][decoder_texture_idct2d] = false;
//	mapping[6][5][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][5][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][5][decoder_motion_interpolation] = false;
//	mapping[6][5][decoder_motion_add] = true;
//	mapping[6][5][decoder_motion_framebuf] = false;
//
//	// Processor index 6
//	mapping[6][6][source] = false;
//	mapping[6][6][display] = false;
//	mapping[6][6][decoder_merger] = false;
//	mapping[6][6][decoder_parser_parseheaders] = false;
//	mapping[6][6][decoder_parser_mvseq] = false;
//	mapping[6][6][decoder_parser_blkexp] = false;
//	mapping[6][6][decoder_parser_mvrecon] = false;
//	mapping[6][6][decoder_texture_DCsplit] = false;
//	mapping[6][6][decoder_texture_IS] = false;
//	mapping[6][6][decoder_texture_IAP] = false;
//	mapping[6][6][decoder_texture_IQ] = false;
//	mapping[6][6][decoder_texture_idct2d] = false;
//	mapping[6][6][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[6][6][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[6][6][decoder_motion_interpolation] = false;
//	mapping[6][6][decoder_motion_add] = false;
//	mapping[6][6][decoder_motion_framebuf] = true;
//}
//
static void setMapping_07(){	// Mapping index 7, 8 processors

	// Processor index 0
	mapping[7][0][decoder_merger] = false;
	mapping[7][0][decoder_parser_parseheaders] = false;
	mapping[7][0][decoder_parser_mvseq] = false;
	mapping[7][0][decoder_parser_blkexp] = false;
	mapping[7][0][decoder_parser_mvrecon] = false;
	mapping[7][0][decoder_texture_DCsplit] = false;
	mapping[7][0][decoder_texture_IS] = false;
	mapping[7][0][decoder_texture_IAP] = false;
	mapping[7][0][decoder_texture_IQ] = false;
	mapping[7][0][decoder_texture_idct2d] = false;
	mapping[7][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][0][decoder_motion_interpolation] = false;
	mapping[7][0][decoder_motion_add] = false;
	mapping[7][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[7][1][decoder_merger] = true;
	mapping[7][1][decoder_parser_parseheaders] = true;
	mapping[7][1][decoder_parser_mvseq] = true;
	mapping[7][1][decoder_parser_blkexp] = true;
	mapping[7][1][decoder_parser_mvrecon] = true;
	mapping[7][1][decoder_texture_DCsplit] = false;
	mapping[7][1][decoder_texture_IS] = false;
	mapping[7][1][decoder_texture_IAP] = false;
	mapping[7][1][decoder_texture_IQ] = false;
	mapping[7][1][decoder_texture_idct2d] = false;
	mapping[7][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][1][decoder_motion_interpolation] = false;
	mapping[7][1][decoder_motion_add] = false;
	mapping[7][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[7][2][decoder_merger] = false;
	mapping[7][2][decoder_parser_parseheaders] = false;
	mapping[7][2][decoder_parser_mvseq] = false;
	mapping[7][2][decoder_parser_blkexp] = false;
	mapping[7][2][decoder_parser_mvrecon] = false;
	mapping[7][2][decoder_texture_DCsplit] = true;
	mapping[7][2][decoder_texture_IS] = true;
	mapping[7][2][decoder_texture_IAP] = false;
	mapping[7][2][decoder_texture_IQ] = false;
	mapping[7][2][decoder_texture_idct2d] = false;
	mapping[7][2][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][2][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][2][decoder_motion_interpolation] = false;
	mapping[7][2][decoder_motion_add] = false;
	mapping[7][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[7][3][decoder_merger] = false;
	mapping[7][3][decoder_parser_parseheaders] = false;
	mapping[7][3][decoder_parser_mvseq] = false;
	mapping[7][3][decoder_parser_blkexp] = false;
	mapping[7][3][decoder_parser_mvrecon] = false;
	mapping[7][3][decoder_texture_DCsplit] = false;
	mapping[7][3][decoder_texture_IS] = false;
	mapping[7][3][decoder_texture_IAP] = true;
	mapping[7][3][decoder_texture_IQ] = true;
	mapping[7][3][decoder_texture_idct2d] = false;
	mapping[7][3][decoder_texture_DCReconstruction_addressing] = true;
	mapping[7][3][decoder_texture_DCReconstruction_invpred] = true;
	mapping[7][3][decoder_motion_interpolation] = false;
	mapping[7][3][decoder_motion_add] = false;
	mapping[7][3][decoder_motion_framebuf] = false;

	// Processor index 4
	mapping[7][4][decoder_merger] = false;
	mapping[7][4][decoder_parser_parseheaders] = false;
	mapping[7][4][decoder_parser_mvseq] = false;
	mapping[7][4][decoder_parser_blkexp] = false;
	mapping[7][4][decoder_parser_mvrecon] = false;
	mapping[7][4][decoder_texture_DCsplit] = false;
	mapping[7][4][decoder_texture_IS] = false;
	mapping[7][4][decoder_texture_IAP] = false;
	mapping[7][4][decoder_texture_IQ] = false;
	mapping[7][4][decoder_texture_idct2d] = true;
	mapping[7][4][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][4][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][4][decoder_motion_interpolation] = false;
	mapping[7][4][decoder_motion_add] = false;
	mapping[7][4][decoder_motion_framebuf] = false;

	// Processor index 5
	mapping[7][5][decoder_merger] = false;
	mapping[7][5][decoder_parser_parseheaders] = false;
	mapping[7][5][decoder_parser_mvseq] = false;
	mapping[7][5][decoder_parser_blkexp] = false;
	mapping[7][5][decoder_parser_mvrecon] = false;
	mapping[7][5][decoder_texture_DCsplit] = false;
	mapping[7][5][decoder_texture_IS] = false;
	mapping[7][5][decoder_texture_IAP] = false;
	mapping[7][5][decoder_texture_IQ] = false;
	mapping[7][5][decoder_texture_idct2d] = false;
	mapping[7][5][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][5][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][5][decoder_motion_interpolation] = true;
	mapping[7][5][decoder_motion_add] = false;
	mapping[7][5][decoder_motion_framebuf] = false;

	// Processor index 6
	mapping[7][6][decoder_merger] = false;
	mapping[7][6][decoder_parser_parseheaders] = false;
	mapping[7][6][decoder_parser_mvseq] = false;
	mapping[7][6][decoder_parser_blkexp] = false;
	mapping[7][6][decoder_parser_mvrecon] = false;
	mapping[7][6][decoder_texture_DCsplit] = false;
	mapping[7][6][decoder_texture_IS] = false;
	mapping[7][6][decoder_texture_IAP] = false;
	mapping[7][6][decoder_texture_IQ] = false;
	mapping[7][6][decoder_texture_idct2d] = false;
	mapping[7][6][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][6][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][6][decoder_motion_interpolation] = false;
	mapping[7][6][decoder_motion_add] = true;
	mapping[7][6][decoder_motion_framebuf] = false;

	// Processor index 7
	mapping[7][7][decoder_merger] = false;
	mapping[7][7][decoder_parser_parseheaders] = false;
	mapping[7][7][decoder_parser_mvseq] = false;
	mapping[7][7][decoder_parser_blkexp] = false;
	mapping[7][7][decoder_parser_mvrecon] = false;
	mapping[7][7][decoder_texture_DCsplit] = false;
	mapping[7][7][decoder_texture_IS] = false;
	mapping[7][7][decoder_texture_IAP] = false;
	mapping[7][7][decoder_texture_IQ] = false;
	mapping[7][7][decoder_texture_idct2d] = false;
	mapping[7][7][decoder_texture_DCReconstruction_addressing] = false;
	mapping[7][7][decoder_texture_DCReconstruction_invpred] = false;
	mapping[7][7][decoder_motion_interpolation] = false;
	mapping[7][7][decoder_motion_add] = false;
	mapping[7][7][decoder_motion_framebuf] = true;
}

//static void setMapping_08(){	// Mapping index 8, 9 processors
//
//	// Processor index 0
//	mapping[8][0][source] = false;
//	mapping[8][0][display] = false;
//	mapping[8][0][decoder_merger] = true;
//	mapping[8][0][decoder_parser_parseheaders] = false;
//	mapping[8][0][decoder_parser_mvseq] = false;
//	mapping[8][0][decoder_parser_blkexp] = false;
//	mapping[8][0][decoder_parser_mvrecon] = false;
//	mapping[8][0][decoder_texture_DCsplit] = false;
//	mapping[8][0][decoder_texture_IS] = false;
//	mapping[8][0][decoder_texture_IAP] = false;
//	mapping[8][0][decoder_texture_IQ] = false;
//	mapping[8][0][decoder_texture_idct2d] = false;
//	mapping[8][0][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][0][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][0][decoder_motion_interpolation] = false;
//	mapping[8][0][decoder_motion_add] = false;
//	mapping[8][0][decoder_motion_framebuf] = false;
//
//	// Processor index 1
//	mapping[8][1][source] = false;
//	mapping[8][1][display] = false;
//	mapping[8][1][decoder_merger] = false;
//	mapping[8][1][decoder_parser_parseheaders] = true;
//	mapping[8][1][decoder_parser_mvseq] = false;
//	mapping[8][1][decoder_parser_blkexp] = false;
//	mapping[8][1][decoder_parser_mvrecon] = false;
//	mapping[8][1][decoder_texture_DCsplit] = false;
//	mapping[8][1][decoder_texture_IS] = false;
//	mapping[8][1][decoder_texture_IAP] = false;
//	mapping[8][1][decoder_texture_IQ] = false;
//	mapping[8][1][decoder_texture_idct2d] = false;
//	mapping[8][1][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][1][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][1][decoder_motion_interpolation] = false;
//	mapping[8][1][decoder_motion_add] = false;
//	mapping[8][1][decoder_motion_framebuf] = false;
//
//	// Processor index 2
//	mapping[8][2][source] = false;
//	mapping[8][2][display] = false;
//	mapping[8][2][decoder_merger] = false;
//	mapping[8][2][decoder_parser_parseheaders] = false;
//	mapping[8][2][decoder_parser_mvseq] = true;
//	mapping[8][2][decoder_parser_blkexp] = true;
//	mapping[8][2][decoder_parser_mvrecon] = true;
//	mapping[8][2][decoder_texture_DCsplit] = false;
//	mapping[8][2][decoder_texture_IS] = false;
//	mapping[8][2][decoder_texture_IAP] = false;
//	mapping[8][2][decoder_texture_IQ] = false;
//	mapping[8][2][decoder_texture_idct2d] = false;
//	mapping[8][2][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][2][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][2][decoder_motion_interpolation] = false;
//	mapping[8][2][decoder_motion_add] = false;
//	mapping[8][2][decoder_motion_framebuf] = false;
//
//	// Processor index 3
//	mapping[8][3][source] = false;
//	mapping[8][3][display] = false;
//	mapping[8][3][decoder_merger] = false;
//	mapping[8][3][decoder_parser_parseheaders] = false;
//	mapping[8][3][decoder_parser_mvseq] = false;
//	mapping[8][3][decoder_parser_blkexp] = false;
//	mapping[8][3][decoder_parser_mvrecon] = false;
//	mapping[8][3][decoder_texture_DCsplit] = true;
//	mapping[8][3][decoder_texture_IS] = true;
//	mapping[8][3][decoder_texture_IAP] = false;
//	mapping[8][3][decoder_texture_IQ] = false;
//	mapping[8][3][decoder_texture_idct2d] = false;
//	mapping[8][3][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][3][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][3][decoder_motion_interpolation] = false;
//	mapping[8][3][decoder_motion_add] = false;
//	mapping[8][3][decoder_motion_framebuf] = false;
//
//	// Processor index 4
//	mapping[8][4][source] = false;
//	mapping[8][4][display] = false;
//	mapping[8][4][decoder_merger] = false;
//	mapping[8][4][decoder_parser_parseheaders] = false;
//	mapping[8][4][decoder_parser_mvseq] = false;
//	mapping[8][4][decoder_parser_blkexp] = false;
//	mapping[8][4][decoder_parser_mvrecon] = false;
//	mapping[8][4][decoder_texture_DCsplit] = false;
//	mapping[8][4][decoder_texture_IS] = false;
//	mapping[8][4][decoder_texture_IAP] = true;
//	mapping[8][4][decoder_texture_IQ] = true;
//	mapping[8][4][decoder_texture_idct2d] = false;
//	mapping[8][4][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][4][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][4][decoder_motion_interpolation] = false;
//	mapping[8][4][decoder_motion_add] = false;
//	mapping[8][4][decoder_motion_framebuf] = false;
//
//	// Processor index 5
//	mapping[8][5][source] = false;
//	mapping[8][5][display] = false;
//	mapping[8][5][decoder_merger] = false;
//	mapping[8][5][decoder_parser_parseheaders] = false;
//	mapping[8][5][decoder_parser_mvseq] = false;
//	mapping[8][5][decoder_parser_blkexp] = false;
//	mapping[8][5][decoder_parser_mvrecon] = false;
//	mapping[8][5][decoder_texture_DCsplit] = false;
//	mapping[8][5][decoder_texture_IS] = false;
//	mapping[8][5][decoder_texture_IAP] = false;
//	mapping[8][5][decoder_texture_IQ] = false;
//	mapping[8][5][decoder_texture_idct2d] = true;
//	mapping[8][5][decoder_texture_DCReconstruction_addressing] = true;
//	mapping[8][5][decoder_texture_DCReconstruction_invpred] = true;
//	mapping[8][5][decoder_motion_interpolation] = false;
//	mapping[8][5][decoder_motion_add] = false;
//	mapping[8][5][decoder_motion_framebuf] = false;
//
//	// Processor index 6
//	mapping[8][6][source] = false;
//	mapping[8][6][display] = false;
//	mapping[8][6][decoder_merger] = false;
//	mapping[8][6][decoder_parser_parseheaders] = false;
//	mapping[8][6][decoder_parser_mvseq] = false;
//	mapping[8][6][decoder_parser_blkexp] = false;
//	mapping[8][6][decoder_parser_mvrecon] = false;
//	mapping[8][6][decoder_texture_DCsplit] = false;
//	mapping[8][6][decoder_texture_IS] = false;
//	mapping[8][6][decoder_texture_IAP] = false;
//	mapping[8][6][decoder_texture_IQ] = false;
//	mapping[8][6][decoder_texture_idct2d] = false;
//	mapping[8][6][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][6][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][6][decoder_motion_interpolation] = true;
//	mapping[8][6][decoder_motion_add] = false;
//	mapping[8][6][decoder_motion_framebuf] = false;
//
//	// Processor index 7
//	mapping[8][7][source] = false;
//	mapping[8][7][display] = false;
//	mapping[8][7][decoder_merger] = false;
//	mapping[8][7][decoder_parser_parseheaders] = false;
//	mapping[8][7][decoder_parser_mvseq] = false;
//	mapping[8][7][decoder_parser_blkexp] = false;
//	mapping[8][7][decoder_parser_mvrecon] = false;
//	mapping[8][7][decoder_texture_DCsplit] = false;
//	mapping[8][7][decoder_texture_IS] = false;
//	mapping[8][7][decoder_texture_IAP] = false;
//	mapping[8][7][decoder_texture_IQ] = false;
//	mapping[8][7][decoder_texture_idct2d] = false;
//	mapping[8][7][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][7][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][7][decoder_motion_interpolation] = false;
//	mapping[8][7][decoder_motion_add] = true;
//	mapping[8][7][decoder_motion_framebuf] = false;
//
//	// Processor index 8
//	mapping[8][8][source] = false;
//	mapping[8][8][display] = false;
//	mapping[8][8][decoder_merger] = false;
//	mapping[8][8][decoder_parser_parseheaders] = false;
//	mapping[8][8][decoder_parser_mvseq] = false;
//	mapping[8][8][decoder_parser_blkexp] = false;
//	mapping[8][8][decoder_parser_mvrecon] = false;
//	mapping[8][8][decoder_texture_DCsplit] = false;
//	mapping[8][8][decoder_texture_IS] = false;
//	mapping[8][8][decoder_texture_IAP] = false;
//	mapping[8][8][decoder_texture_IQ] = false;
//	mapping[8][8][decoder_texture_idct2d] = false;
//	mapping[8][8][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[8][8][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[8][8][decoder_motion_interpolation] = false;
//	mapping[8][8][decoder_motion_add] = false;
//	mapping[8][8][decoder_motion_framebuf] = true;
//}
//
//static void setMapping_09(){	// Mapping index 9, 10 processors
//
//	// Processor index 0
//	mapping[9][0][source] = false;
//	mapping[9][0][display] = false;
//	mapping[9][0][decoder_merger] = true;
//	mapping[9][0][decoder_parser_parseheaders] = false;
//	mapping[9][0][decoder_parser_mvseq] = false;
//	mapping[9][0][decoder_parser_blkexp] = false;
//	mapping[9][0][decoder_parser_mvrecon] = false;
//	mapping[9][0][decoder_texture_DCsplit] = false;
//	mapping[9][0][decoder_texture_IS] = false;
//	mapping[9][0][decoder_texture_IAP] = false;
//	mapping[9][0][decoder_texture_IQ] = false;
//	mapping[9][0][decoder_texture_idct2d] = false;
//	mapping[9][0][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][0][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][0][decoder_motion_interpolation] = false;
//	mapping[9][0][decoder_motion_add] = false;
//	mapping[9][0][decoder_motion_framebuf] = false;
//
//	// Processor index 1
//	mapping[9][1][source] = false;
//	mapping[9][1][display] = false;
//	mapping[9][1][decoder_merger] = false;
//	mapping[9][1][decoder_parser_parseheaders] = true;
//	mapping[9][1][decoder_parser_mvseq] = false;
//	mapping[9][1][decoder_parser_blkexp] = false;
//	mapping[9][1][decoder_parser_mvrecon] = false;
//	mapping[9][1][decoder_texture_DCsplit] = false;
//	mapping[9][1][decoder_texture_IS] = false;
//	mapping[9][1][decoder_texture_IAP] = false;
//	mapping[9][1][decoder_texture_IQ] = false;
//	mapping[9][1][decoder_texture_idct2d] = false;
//	mapping[9][1][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][1][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][1][decoder_motion_interpolation] = false;
//	mapping[9][1][decoder_motion_add] = false;
//	mapping[9][1][decoder_motion_framebuf] = false;
//
//	// Processor index 2
//	mapping[9][2][source] = false;
//	mapping[9][2][display] = false;
//	mapping[9][2][decoder_merger] = false;
//	mapping[9][2][decoder_parser_parseheaders] = false;
//	mapping[9][2][decoder_parser_mvseq] = true;
//	mapping[9][2][decoder_parser_blkexp] = true;
//	mapping[9][2][decoder_parser_mvrecon] = true;
//	mapping[9][2][decoder_texture_DCsplit] = false;
//	mapping[9][2][decoder_texture_IS] = false;
//	mapping[9][2][decoder_texture_IAP] = false;
//	mapping[9][2][decoder_texture_IQ] = false;
//	mapping[9][2][decoder_texture_idct2d] = false;
//	mapping[9][2][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][2][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][2][decoder_motion_interpolation] = false;
//	mapping[9][2][decoder_motion_add] = false;
//	mapping[9][2][decoder_motion_framebuf] = false;
//
//	// Processor index 3
//	mapping[9][3][source] = false;
//	mapping[9][3][display] = false;
//	mapping[9][3][decoder_merger] = false;
//	mapping[9][3][decoder_parser_parseheaders] = false;
//	mapping[9][3][decoder_parser_mvseq] = false;
//	mapping[9][3][decoder_parser_blkexp] = false;
//	mapping[9][3][decoder_parser_mvrecon] = false;
//	mapping[9][3][decoder_texture_DCsplit] = true;
//	mapping[9][3][decoder_texture_IS] = true;
//	mapping[9][3][decoder_texture_IAP] = false;
//	mapping[9][3][decoder_texture_IQ] = false;
//	mapping[9][3][decoder_texture_idct2d] = false;
//	mapping[9][3][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][3][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][3][decoder_motion_interpolation] = false;
//	mapping[9][3][decoder_motion_add] = false;
//	mapping[9][3][decoder_motion_framebuf] = false;
//
//	// Processor index 4
//	mapping[9][4][source] = false;
//	mapping[9][4][display] = false;
//	mapping[9][4][decoder_merger] = false;
//	mapping[9][4][decoder_parser_parseheaders] = false;
//	mapping[9][4][decoder_parser_mvseq] = false;
//	mapping[9][4][decoder_parser_blkexp] = false;
//	mapping[9][4][decoder_parser_mvrecon] = false;
//	mapping[9][4][decoder_texture_DCsplit] = false;
//	mapping[9][4][decoder_texture_IS] = false;
//	mapping[9][4][decoder_texture_IAP] = true;
//	mapping[9][4][decoder_texture_IQ] = true;
//	mapping[9][4][decoder_texture_idct2d] = false;
//	mapping[9][4][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][4][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][4][decoder_motion_interpolation] = false;
//	mapping[9][4][decoder_motion_add] = false;
//	mapping[9][4][decoder_motion_framebuf] = false;
//
//	// Processor index 5
//	mapping[9][5][source] = false;
//	mapping[9][5][display] = false;
//	mapping[9][5][decoder_merger] = false;
//	mapping[9][5][decoder_parser_parseheaders] = false;
//	mapping[9][5][decoder_parser_mvseq] = false;
//	mapping[9][5][decoder_parser_blkexp] = false;
//	mapping[9][5][decoder_parser_mvrecon] = false;
//	mapping[9][5][decoder_texture_DCsplit] = false;
//	mapping[9][5][decoder_texture_IS] = false;
//	mapping[9][5][decoder_texture_IAP] = false;
//	mapping[9][5][decoder_texture_IQ] = false;
//	mapping[9][5][decoder_texture_idct2d] = true;
//	mapping[9][5][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][5][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][5][decoder_motion_interpolation] = false;
//	mapping[9][5][decoder_motion_add] = false;
//	mapping[9][5][decoder_motion_framebuf] = false;
//
//	// Processor index 6
//	mapping[9][6][source] = false;
//	mapping[9][6][display] = false;
//	mapping[9][6][decoder_merger] = false;
//	mapping[9][6][decoder_parser_parseheaders] = false;
//	mapping[9][6][decoder_parser_mvseq] = false;
//	mapping[9][6][decoder_parser_blkexp] = false;
//	mapping[9][6][decoder_parser_mvrecon] = false;
//	mapping[9][6][decoder_texture_DCsplit] = false;
//	mapping[9][6][decoder_texture_IS] = false;
//	mapping[9][6][decoder_texture_IAP] = false;
//	mapping[9][6][decoder_texture_IQ] = false;
//	mapping[9][6][decoder_texture_idct2d] = false;
//	mapping[9][6][decoder_texture_DCReconstruction_addressing] = true;
//	mapping[9][6][decoder_texture_DCReconstruction_invpred] = true;
//	mapping[9][6][decoder_motion_interpolation] = false;
//	mapping[9][6][decoder_motion_add] = false;
//	mapping[9][6][decoder_motion_framebuf] = false;
//
//	// Processor index 7
//	mapping[9][7][source] = false;
//	mapping[9][7][display] = false;
//	mapping[9][7][decoder_merger] = false;
//	mapping[9][7][decoder_parser_parseheaders] = false;
//	mapping[9][7][decoder_parser_mvseq] = false;
//	mapping[9][7][decoder_parser_blkexp] = false;
//	mapping[9][7][decoder_parser_mvrecon] = false;
//	mapping[9][7][decoder_texture_DCsplit] = false;
//	mapping[9][7][decoder_texture_IS] = false;
//	mapping[9][7][decoder_texture_IAP] = false;
//	mapping[9][7][decoder_texture_IQ] = false;
//	mapping[9][7][decoder_texture_idct2d] = false;
//	mapping[9][7][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][7][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][7][decoder_motion_interpolation] = true;
//	mapping[9][7][decoder_motion_add] = false;
//	mapping[9][7][decoder_motion_framebuf] = false;
//
//	// Processor index 8
//	mapping[9][8][source] = false;
//	mapping[9][8][display] = false;
//	mapping[9][8][decoder_merger] = false;
//	mapping[9][8][decoder_parser_parseheaders] = false;
//	mapping[9][8][decoder_parser_mvseq] = false;
//	mapping[9][8][decoder_parser_blkexp] = false;
//	mapping[9][8][decoder_parser_mvrecon] = false;
//	mapping[9][8][decoder_texture_DCsplit] = false;
//	mapping[9][8][decoder_texture_IS] = false;
//	mapping[9][8][decoder_texture_IAP] = false;
//	mapping[9][8][decoder_texture_IQ] = false;
//	mapping[9][8][decoder_texture_idct2d] = false;
//	mapping[9][8][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][8][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][8][decoder_motion_interpolation] = false;
//	mapping[9][8][decoder_motion_add] = true;
//	mapping[9][8][decoder_motion_framebuf] = false;
//
//	// Processor index 9
//	mapping[9][9][source] = false;
//	mapping[9][9][display] = false;
//	mapping[9][9][decoder_merger] = false;
//	mapping[9][9][decoder_parser_parseheaders] = false;
//	mapping[9][9][decoder_parser_mvseq] = false;
//	mapping[9][9][decoder_parser_blkexp] = false;
//	mapping[9][9][decoder_parser_mvrecon] = false;
//	mapping[9][9][decoder_texture_DCsplit] = false;
//	mapping[9][9][decoder_texture_IS] = false;
//	mapping[9][9][decoder_texture_IAP] = false;
//	mapping[9][9][decoder_texture_IQ] = false;
//	mapping[9][9][decoder_texture_idct2d] = false;
//	mapping[9][9][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[9][9][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[9][9][decoder_motion_interpolation] = false;
//	mapping[9][9][decoder_motion_add] = false;
//	mapping[9][9][decoder_motion_framebuf] = true;
//}
//
//static void setMapping_10(){	// Mapping index 10, 11 processors
//
//	// Processor index 0
//	mapping[10][0][source] = false;
//	mapping[10][0][display] = false;
//	mapping[10][0][decoder_merger] = true;
//	mapping[10][0][decoder_parser_parseheaders] = false;
//	mapping[10][0][decoder_parser_mvseq] = false;
//	mapping[10][0][decoder_parser_blkexp] = false;
//	mapping[10][0][decoder_parser_mvrecon] = false;
//	mapping[10][0][decoder_texture_DCsplit] = false;
//	mapping[10][0][decoder_texture_IS] = false;
//	mapping[10][0][decoder_texture_IAP] = false;
//	mapping[10][0][decoder_texture_IQ] = false;
//	mapping[10][0][decoder_texture_idct2d] = false;
//	mapping[10][0][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][0][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][0][decoder_motion_interpolation] = false;
//	mapping[10][0][decoder_motion_add] = false;
//	mapping[10][0][decoder_motion_framebuf] = false;
//
//	// Processor index 1
//	mapping[10][1][source] = false;
//	mapping[10][1][display] = false;
//	mapping[10][1][decoder_merger] = false;
//	mapping[10][1][decoder_parser_parseheaders] = true;
//	mapping[10][1][decoder_parser_mvseq] = false;
//	mapping[10][1][decoder_parser_blkexp] = false;
//	mapping[10][1][decoder_parser_mvrecon] = false;
//	mapping[10][1][decoder_texture_DCsplit] = false;
//	mapping[10][1][decoder_texture_IS] = false;
//	mapping[10][1][decoder_texture_IAP] = false;
//	mapping[10][1][decoder_texture_IQ] = false;
//	mapping[10][1][decoder_texture_idct2d] = false;
//	mapping[10][1][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][1][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][1][decoder_motion_interpolation] = false;
//	mapping[10][1][decoder_motion_add] = false;
//	mapping[10][1][decoder_motion_framebuf] = false;
//
//	// Processor index 2
//	mapping[10][2][source] = false;
//	mapping[10][2][display] = false;
//	mapping[10][2][decoder_merger] = false;
//	mapping[10][2][decoder_parser_parseheaders] = false;
//	mapping[10][2][decoder_parser_mvseq] = true;
//	mapping[10][2][decoder_parser_blkexp] = true;
//	mapping[10][2][decoder_parser_mvrecon] = true;
//	mapping[10][2][decoder_texture_DCsplit] = false;
//	mapping[10][2][decoder_texture_IS] = false;
//	mapping[10][2][decoder_texture_IAP] = false;
//	mapping[10][2][decoder_texture_IQ] = false;
//	mapping[10][2][decoder_texture_idct2d] = false;
//	mapping[10][2][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][2][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][2][decoder_motion_interpolation] = false;
//	mapping[10][2][decoder_motion_add] = false;
//	mapping[10][2][decoder_motion_framebuf] = false;
//
//	// Processor index 3
//	mapping[10][3][source] = false;
//	mapping[10][3][display] = false;
//	mapping[10][3][decoder_merger] = false;
//	mapping[10][3][decoder_parser_parseheaders] = false;
//	mapping[10][3][decoder_parser_mvseq] = false;
//	mapping[10][3][decoder_parser_blkexp] = false;
//	mapping[10][3][decoder_parser_mvrecon] = false;
//	mapping[10][3][decoder_texture_DCsplit] = true;
//	mapping[10][3][decoder_texture_IS] = true;
//	mapping[10][3][decoder_texture_IAP] = false;
//	mapping[10][3][decoder_texture_IQ] = false;
//	mapping[10][3][decoder_texture_idct2d] = false;
//	mapping[10][3][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][3][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][3][decoder_motion_interpolation] = false;
//	mapping[10][3][decoder_motion_add] = false;
//	mapping[10][3][decoder_motion_framebuf] = false;
//
//	// Processor index 4
//	mapping[10][4][source] = false;
//	mapping[10][4][display] = false;
//	mapping[10][4][decoder_merger] = false;
//	mapping[10][4][decoder_parser_parseheaders] = false;
//	mapping[10][4][decoder_parser_mvseq] = false;
//	mapping[10][4][decoder_parser_blkexp] = false;
//	mapping[10][4][decoder_parser_mvrecon] = false;
//	mapping[10][4][decoder_texture_DCsplit] = false;
//	mapping[10][4][decoder_texture_IS] = false;
//	mapping[10][4][decoder_texture_IAP] = true;
//	mapping[10][4][decoder_texture_IQ] = false;
//	mapping[10][4][decoder_texture_idct2d] = false;
//	mapping[10][4][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][4][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][4][decoder_motion_interpolation] = false;
//	mapping[10][4][decoder_motion_add] = false;
//	mapping[10][4][decoder_motion_framebuf] = false;
//
//	// Processor index 5
//	mapping[10][5][source] = false;
//	mapping[10][5][display] = false;
//	mapping[10][5][decoder_merger] = false;
//	mapping[10][5][decoder_parser_parseheaders] = false;
//	mapping[10][5][decoder_parser_mvseq] = false;
//	mapping[10][5][decoder_parser_blkexp] = false;
//	mapping[10][5][decoder_parser_mvrecon] = false;
//	mapping[10][5][decoder_texture_DCsplit] = false;
//	mapping[10][5][decoder_texture_IS] = false;
//	mapping[10][5][decoder_texture_IAP] = false;
//	mapping[10][5][decoder_texture_IQ] = true;
//	mapping[10][5][decoder_texture_idct2d] = false;
//	mapping[10][5][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][5][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][5][decoder_motion_interpolation] = false;
//	mapping[10][5][decoder_motion_add] = false;
//	mapping[10][5][decoder_motion_framebuf] = false;
//
//	// Processor index 6
//	mapping[10][6][source] = false;
//	mapping[10][6][display] = false;
//	mapping[10][6][decoder_merger] = false;
//	mapping[10][6][decoder_parser_parseheaders] = false;
//	mapping[10][6][decoder_parser_mvseq] = false;
//	mapping[10][6][decoder_parser_blkexp] = false;
//	mapping[10][6][decoder_parser_mvrecon] = false;
//	mapping[10][6][decoder_texture_DCsplit] = false;
//	mapping[10][6][decoder_texture_IS] = false;
//	mapping[10][6][decoder_texture_IAP] = false;
//	mapping[10][6][decoder_texture_IQ] = false;
//	mapping[10][6][decoder_texture_idct2d] = true;
//	mapping[10][6][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][6][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][6][decoder_motion_interpolation] = false;
//	mapping[10][6][decoder_motion_add] = false;
//	mapping[10][6][decoder_motion_framebuf] = false;
//
//	// Processor index 7
//	mapping[10][7][source] = false;
//	mapping[10][7][display] = false;
//	mapping[10][7][decoder_merger] = false;
//	mapping[10][7][decoder_parser_parseheaders] = false;
//	mapping[10][7][decoder_parser_mvseq] = false;
//	mapping[10][7][decoder_parser_blkexp] = false;
//	mapping[10][7][decoder_parser_mvrecon] = false;
//	mapping[10][7][decoder_texture_DCsplit] = false;
//	mapping[10][7][decoder_texture_IS] = false;
//	mapping[10][7][decoder_texture_IAP] = false;
//	mapping[10][7][decoder_texture_IQ] = false;
//	mapping[10][7][decoder_texture_idct2d] = false;
//	mapping[10][7][decoder_texture_DCReconstruction_addressing] = true;
//	mapping[10][7][decoder_texture_DCReconstruction_invpred] = true;
//	mapping[10][7][decoder_motion_interpolation] = false;
//	mapping[10][7][decoder_motion_add] = false;
//	mapping[10][7][decoder_motion_framebuf] = false;
//
//	// Processor index 8
//	mapping[10][8][source] = false;
//	mapping[10][8][display] = false;
//	mapping[10][8][decoder_merger] = false;
//	mapping[10][8][decoder_parser_parseheaders] = false;
//	mapping[10][8][decoder_parser_mvseq] = false;
//	mapping[10][8][decoder_parser_blkexp] = false;
//	mapping[10][8][decoder_parser_mvrecon] = false;
//	mapping[10][8][decoder_texture_DCsplit] = false;
//	mapping[10][8][decoder_texture_IS] = false;
//	mapping[10][8][decoder_texture_IAP] = false;
//	mapping[10][8][decoder_texture_IQ] = false;
//	mapping[10][8][decoder_texture_idct2d] = false;
//	mapping[10][8][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][8][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][8][decoder_motion_interpolation] = true;
//	mapping[10][8][decoder_motion_add] = false;
//	mapping[10][8][decoder_motion_framebuf] = false;
//
//	// Processor index 9
//	mapping[10][9][source] = false;
//	mapping[10][9][display] = false;
//	mapping[10][9][decoder_merger] = false;
//	mapping[10][9][decoder_parser_parseheaders] = false;
//	mapping[10][9][decoder_parser_mvseq] = false;
//	mapping[10][9][decoder_parser_blkexp] = false;
//	mapping[10][9][decoder_parser_mvrecon] = false;
//	mapping[10][9][decoder_texture_DCsplit] = false;
//	mapping[10][9][decoder_texture_IS] = false;
//	mapping[10][9][decoder_texture_IAP] = false;
//	mapping[10][9][decoder_texture_IQ] = false;
//	mapping[10][9][decoder_texture_idct2d] = false;
//	mapping[10][9][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][9][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][9][decoder_motion_interpolation] = false;
//	mapping[10][9][decoder_motion_add] = true;
//	mapping[10][9][decoder_motion_framebuf] = false;
//
//	// Processor index 10
//	mapping[10][10][source] = false;
//	mapping[10][10][display] = false;
//	mapping[10][10][decoder_merger] = false;
//	mapping[10][10][decoder_parser_parseheaders] = false;
//	mapping[10][10][decoder_parser_mvseq] = false;
//	mapping[10][10][decoder_parser_blkexp] = false;
//	mapping[10][10][decoder_parser_mvrecon] = false;
//	mapping[10][10][decoder_texture_DCsplit] = false;
//	mapping[10][10][decoder_texture_IS] = false;
//	mapping[10][10][decoder_texture_IAP] = false;
//	mapping[10][10][decoder_texture_IQ] = false;
//	mapping[10][10][decoder_texture_idct2d] = false;
//	mapping[10][10][decoder_texture_DCReconstruction_addressing] = false;
//	mapping[10][10][decoder_texture_DCReconstruction_invpred] = false;
//	mapping[10][10][decoder_motion_interpolation] = false;
//	mapping[10][10][decoder_motion_add] = false;
//	mapping[10][10][decoder_motion_framebuf] = true;
//}
//
static void setMapping_11(){	// Mapping index 11, 12 processors

	// Processor index 0
	mapping[11][0][decoder_merger] = false;
	mapping[11][0][decoder_parser_parseheaders] = false;
	mapping[11][0][decoder_parser_mvseq] = false;
	mapping[11][0][decoder_parser_blkexp] = false;
	mapping[11][0][decoder_parser_mvrecon] = false;
	mapping[11][0][decoder_texture_DCsplit] = false;
	mapping[11][0][decoder_texture_IS] = false;
	mapping[11][0][decoder_texture_IAP] = false;
	mapping[11][0][decoder_texture_IQ] = false;
	mapping[11][0][decoder_texture_idct2d] = false;
	mapping[11][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][0][decoder_motion_interpolation] = false;
	mapping[11][0][decoder_motion_add] = false;
	mapping[11][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[11][1][decoder_merger] = true;
	mapping[11][1][decoder_parser_parseheaders] = true;
	mapping[11][1][decoder_parser_mvseq] = false;
	mapping[11][1][decoder_parser_blkexp] = false;
	mapping[11][1][decoder_parser_mvrecon] = false;
	mapping[11][1][decoder_texture_DCsplit] = false;
	mapping[11][1][decoder_texture_IS] = false;
	mapping[11][1][decoder_texture_IAP] = false;
	mapping[11][1][decoder_texture_IQ] = false;
	mapping[11][1][decoder_texture_idct2d] = false;
	mapping[11][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][1][decoder_motion_interpolation] = false;
	mapping[11][1][decoder_motion_add] = false;
	mapping[11][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[11][2][decoder_merger] = false;
	mapping[11][2][decoder_parser_parseheaders] = false;
	mapping[11][2][decoder_parser_mvseq] = true;
	mapping[11][2][decoder_parser_blkexp] = true;
	mapping[11][2][decoder_parser_mvrecon] = true;
	mapping[11][2][decoder_texture_DCsplit] = false;
	mapping[11][2][decoder_texture_IS] = false;
	mapping[11][2][decoder_texture_IAP] = false;
	mapping[11][2][decoder_texture_IQ] = false;
	mapping[11][2][decoder_texture_idct2d] = false;
	mapping[11][2][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][2][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][2][decoder_motion_interpolation] = false;
	mapping[11][2][decoder_motion_add] = false;
	mapping[11][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[11][3][decoder_merger] = false;
	mapping[11][3][decoder_parser_parseheaders] = false;
	mapping[11][3][decoder_parser_mvseq] = false;
	mapping[11][3][decoder_parser_blkexp] = false;
	mapping[11][3][decoder_parser_mvrecon] = false;
	mapping[11][3][decoder_texture_DCsplit] = true;
	mapping[11][3][decoder_texture_IS] = false;
	mapping[11][3][decoder_texture_IAP] = false;
	mapping[11][3][decoder_texture_IQ] = false;
	mapping[11][3][decoder_texture_idct2d] = false;
	mapping[11][3][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][3][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][3][decoder_motion_interpolation] = false;
	mapping[11][3][decoder_motion_add] = false;
	mapping[11][3][decoder_motion_framebuf] = false;

	// Processor index 4
	mapping[11][4][decoder_merger] = false;
	mapping[11][4][decoder_parser_parseheaders] = false;
	mapping[11][4][decoder_parser_mvseq] = false;
	mapping[11][4][decoder_parser_blkexp] = false;
	mapping[11][4][decoder_parser_mvrecon] = false;
	mapping[11][4][decoder_texture_DCsplit] = false;
	mapping[11][4][decoder_texture_IS] = true;
	mapping[11][4][decoder_texture_IAP] = false;
	mapping[11][4][decoder_texture_IQ] = false;
	mapping[11][4][decoder_texture_idct2d] = false;
	mapping[11][4][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][4][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][4][decoder_motion_interpolation] = false;
	mapping[11][4][decoder_motion_add] = false;
	mapping[11][4][decoder_motion_framebuf] = false;

	// Processor index 5
	mapping[11][5][decoder_merger] = false;
	mapping[11][5][decoder_parser_parseheaders] = false;
	mapping[11][5][decoder_parser_mvseq] = false;
	mapping[11][5][decoder_parser_blkexp] = false;
	mapping[11][5][decoder_parser_mvrecon] = false;
	mapping[11][5][decoder_texture_DCsplit] = false;
	mapping[11][5][decoder_texture_IS] = false;
	mapping[11][5][decoder_texture_IAP] = true;
	mapping[11][5][decoder_texture_IQ] = false;
	mapping[11][5][decoder_texture_idct2d] = false;
	mapping[11][5][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][5][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][5][decoder_motion_interpolation] = false;
	mapping[11][5][decoder_motion_add] = false;
	mapping[11][5][decoder_motion_framebuf] = false;

	// Processor index 6
	mapping[11][6][decoder_merger] = false;
	mapping[11][6][decoder_parser_parseheaders] = false;
	mapping[11][6][decoder_parser_mvseq] = false;
	mapping[11][6][decoder_parser_blkexp] = false;
	mapping[11][6][decoder_parser_mvrecon] = false;
	mapping[11][6][decoder_texture_DCsplit] = false;
	mapping[11][6][decoder_texture_IS] = false;
	mapping[11][6][decoder_texture_IAP] = false;
	mapping[11][6][decoder_texture_IQ] = true;
	mapping[11][6][decoder_texture_idct2d] = false;
	mapping[11][6][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][6][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][6][decoder_motion_interpolation] = false;
	mapping[11][6][decoder_motion_add] = false;
	mapping[11][6][decoder_motion_framebuf] = false;

	// Processor index 7
	mapping[11][7][decoder_merger] = false;
	mapping[11][7][decoder_parser_parseheaders] = false;
	mapping[11][7][decoder_parser_mvseq] = false;
	mapping[11][7][decoder_parser_blkexp] = false;
	mapping[11][7][decoder_parser_mvrecon] = false;
	mapping[11][7][decoder_texture_DCsplit] = false;
	mapping[11][7][decoder_texture_IS] = false;
	mapping[11][7][decoder_texture_IAP] = false;
	mapping[11][7][decoder_texture_IQ] = false;
	mapping[11][7][decoder_texture_idct2d] = true;
	mapping[11][7][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][7][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][7][decoder_motion_interpolation] = false;
	mapping[11][7][decoder_motion_add] = false;
	mapping[11][7][decoder_motion_framebuf] = false;

	// Processor index 8
	mapping[11][8][decoder_merger] = false;
	mapping[11][8][decoder_parser_parseheaders] = false;
	mapping[11][8][decoder_parser_mvseq] = false;
	mapping[11][8][decoder_parser_blkexp] = false;
	mapping[11][8][decoder_parser_mvrecon] = false;
	mapping[11][8][decoder_texture_DCsplit] = false;
	mapping[11][8][decoder_texture_IS] = false;
	mapping[11][8][decoder_texture_IAP] = false;
	mapping[11][8][decoder_texture_IQ] = false;
	mapping[11][8][decoder_texture_idct2d] = false;
	mapping[11][8][decoder_texture_DCReconstruction_addressing] = true;
	mapping[11][8][decoder_texture_DCReconstruction_invpred] = true;
	mapping[11][8][decoder_motion_interpolation] = false;
	mapping[11][8][decoder_motion_add] = false;
	mapping[11][8][decoder_motion_framebuf] = false;

	// Processor index 9
	mapping[11][9][decoder_merger] = false;
	mapping[11][9][decoder_parser_parseheaders] = false;
	mapping[11][9][decoder_parser_mvseq] = false;
	mapping[11][9][decoder_parser_blkexp] = false;
	mapping[11][9][decoder_parser_mvrecon] = false;
	mapping[11][9][decoder_texture_DCsplit] = false;
	mapping[11][9][decoder_texture_IS] = false;
	mapping[11][9][decoder_texture_IAP] = false;
	mapping[11][9][decoder_texture_IQ] = false;
	mapping[11][9][decoder_texture_idct2d] = false;
	mapping[11][9][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][9][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][9][decoder_motion_interpolation] = true;
	mapping[11][9][decoder_motion_add] = false;
	mapping[11][9][decoder_motion_framebuf] = false;

	// Processor index 10
	mapping[11][10][decoder_merger] = false;
	mapping[11][10][decoder_parser_parseheaders] = false;
	mapping[11][10][decoder_parser_mvseq] = false;
	mapping[11][10][decoder_parser_blkexp] = false;
	mapping[11][10][decoder_parser_mvrecon] = false;
	mapping[11][10][decoder_texture_DCsplit] = false;
	mapping[11][10][decoder_texture_IS] = false;
	mapping[11][10][decoder_texture_IAP] = false;
	mapping[11][10][decoder_texture_IQ] = false;
	mapping[11][10][decoder_texture_idct2d] = false;
	mapping[11][10][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][10][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][10][decoder_motion_interpolation] = false;
	mapping[11][10][decoder_motion_add] = true;
	mapping[11][10][decoder_motion_framebuf] = false;

	// Processor index 11
	mapping[11][11][decoder_merger] = false;
	mapping[11][11][decoder_parser_parseheaders] = false;
	mapping[11][11][decoder_parser_mvseq] = false;
	mapping[11][11][decoder_parser_blkexp] = false;
	mapping[11][11][decoder_parser_mvrecon] = false;
	mapping[11][11][decoder_texture_DCsplit] = false;
	mapping[11][11][decoder_texture_IS] = false;
	mapping[11][11][decoder_texture_IAP] = false;
	mapping[11][11][decoder_texture_IQ] = false;
	mapping[11][11][decoder_texture_idct2d] = false;
	mapping[11][11][decoder_texture_DCReconstruction_addressing] = false;
	mapping[11][11][decoder_texture_DCReconstruction_invpred] = false;
	mapping[11][11][decoder_motion_interpolation] = false;
	mapping[11][11][decoder_motion_add] = false;
	mapping[11][11][decoder_motion_framebuf] = true;
}

static void setMapping_12(){	// Mapping index 12, 13 processors.

	// Processor index 0
	mapping[12][0][decoder_merger] = false;
	mapping[12][0][decoder_parser_parseheaders] = false;
	mapping[12][0][decoder_parser_mvseq] = false;
	mapping[12][0][decoder_parser_blkexp] = false;
	mapping[12][0][decoder_parser_mvrecon] = false;
	mapping[12][0][decoder_texture_DCsplit] = false;
	mapping[12][0][decoder_texture_IS] = false;
	mapping[12][0][decoder_texture_IAP] = false;
	mapping[12][0][decoder_texture_IQ] = false;
	mapping[12][0][decoder_texture_idct2d] = false;
	mapping[12][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][0][decoder_motion_interpolation] = false;
	mapping[12][0][decoder_motion_add] = false;
	mapping[12][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[12][1][decoder_merger] = true;
	mapping[12][1][decoder_parser_parseheaders] = false;
	mapping[12][1][decoder_parser_mvseq] = false;
	mapping[12][1][decoder_parser_blkexp] = false;
	mapping[12][1][decoder_parser_mvrecon] = false;
	mapping[12][1][decoder_texture_DCsplit] = false;
	mapping[12][1][decoder_texture_IS] = false;
	mapping[12][1][decoder_texture_IAP] = false;
	mapping[12][1][decoder_texture_IQ] = false;
	mapping[12][1][decoder_texture_idct2d] = false;
	mapping[12][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][1][decoder_motion_interpolation] = false;
	mapping[12][1][decoder_motion_add] = false;
	mapping[12][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[12][2][decoder_merger] = false;
	mapping[12][2][decoder_parser_parseheaders] = true;
	mapping[12][2][decoder_parser_mvseq] = false;
	mapping[12][2][decoder_parser_blkexp] = false;
	mapping[12][2][decoder_parser_mvrecon] = true;
	mapping[12][2][decoder_texture_DCsplit] = false;
	mapping[12][2][decoder_texture_IS] = false;
	mapping[12][2][decoder_texture_IAP] = true;
	mapping[12][2][decoder_texture_IQ] = false;
	mapping[12][2][decoder_texture_idct2d] = false;
	mapping[12][2][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][2][decoder_texture_DCReconstruction_invpred] = true;
	mapping[12][2][decoder_motion_interpolation] = false;
	mapping[12][2][decoder_motion_add] = false;
	mapping[12][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[12][3][decoder_merger] = false;
	mapping[12][3][decoder_parser_parseheaders] = false;
	mapping[12][3][decoder_parser_mvseq] = true;
	mapping[12][3][decoder_parser_blkexp] = false;
	mapping[12][3][decoder_parser_mvrecon] = false;
	mapping[12][3][decoder_texture_DCsplit] = false;
	mapping[12][3][decoder_texture_IS] = false;
	mapping[12][3][decoder_texture_IAP] = false;
	mapping[12][3][decoder_texture_IQ] = false;
	mapping[12][3][decoder_texture_idct2d] = false;
	mapping[12][3][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][3][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][3][decoder_motion_interpolation] = false;
	mapping[12][3][decoder_motion_add] = false;
	mapping[12][3][decoder_motion_framebuf] = false;

	// Processor index 4
	mapping[12][4][decoder_merger] = false;
	mapping[12][4][decoder_parser_parseheaders] = false;
	mapping[12][4][decoder_parser_mvseq] = false;
	mapping[12][4][decoder_parser_blkexp] = true;
	mapping[12][4][decoder_parser_mvrecon] = false;
	mapping[12][4][decoder_texture_DCsplit] = false;
	mapping[12][4][decoder_texture_IS] = false;
	mapping[12][4][decoder_texture_IAP] = false;
	mapping[12][4][decoder_texture_IQ] = false;
	mapping[12][4][decoder_texture_idct2d] = false;
	mapping[12][4][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][4][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][4][decoder_motion_interpolation] = false;
	mapping[12][4][decoder_motion_add] = false;
	mapping[12][4][decoder_motion_framebuf] = false;

	// Processor index 5
	mapping[12][5][decoder_merger] = false;
	mapping[12][5][decoder_parser_parseheaders] = false;
	mapping[12][5][decoder_parser_mvseq] = false;
	mapping[12][5][decoder_parser_blkexp] = false;
	mapping[12][5][decoder_parser_mvrecon] = false;
	mapping[12][5][decoder_texture_DCsplit] = true;
	mapping[12][5][decoder_texture_IS] = false;
	mapping[12][5][decoder_texture_IAP] = false;
	mapping[12][5][decoder_texture_IQ] = false;
	mapping[12][5][decoder_texture_idct2d] = false;
	mapping[12][5][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][5][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][5][decoder_motion_interpolation] = false;
	mapping[12][5][decoder_motion_add] = false;
	mapping[12][5][decoder_motion_framebuf] = false;

	// Processor index 6
	mapping[12][6][decoder_merger] = false;
	mapping[12][6][decoder_parser_parseheaders] = false;
	mapping[12][6][decoder_parser_mvseq] = false;
	mapping[12][6][decoder_parser_blkexp] = false;
	mapping[12][6][decoder_parser_mvrecon] = false;
	mapping[12][6][decoder_texture_DCsplit] = false;
	mapping[12][6][decoder_texture_IS] = true;
	mapping[12][6][decoder_texture_IAP] = false;
	mapping[12][6][decoder_texture_IQ] = false;
	mapping[12][6][decoder_texture_idct2d] = false;
	mapping[12][6][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][6][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][6][decoder_motion_interpolation] = false;
	mapping[12][6][decoder_motion_add] = false;
	mapping[12][6][decoder_motion_framebuf] = false;

	// Processor index 7
	mapping[12][7][decoder_merger] = false;
	mapping[12][7][decoder_parser_parseheaders] = false;
	mapping[12][7][decoder_parser_mvseq] = false;
	mapping[12][7][decoder_parser_blkexp] = false;
	mapping[12][7][decoder_parser_mvrecon] = false;
	mapping[12][7][decoder_texture_DCsplit] = false;
	mapping[12][7][decoder_texture_IS] = false;
	mapping[12][7][decoder_texture_IAP] = false;
	mapping[12][7][decoder_texture_IQ] = true;
	mapping[12][7][decoder_texture_idct2d] = false;
	mapping[12][7][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][7][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][7][decoder_motion_interpolation] = false;
	mapping[12][7][decoder_motion_add] = false;
	mapping[12][7][decoder_motion_framebuf] = false;

	// Processor index 8
	mapping[12][8][decoder_merger] = false;
	mapping[12][8][decoder_parser_parseheaders] = false;
	mapping[12][8][decoder_parser_mvseq] = false;
	mapping[12][8][decoder_parser_blkexp] = false;
	mapping[12][8][decoder_parser_mvrecon] = false;
	mapping[12][8][decoder_texture_DCsplit] = false;
	mapping[12][8][decoder_texture_IS] = false;
	mapping[12][8][decoder_texture_IAP] = false;
	mapping[12][8][decoder_texture_IQ] = false;
	mapping[12][8][decoder_texture_idct2d] = true;
	mapping[12][8][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][8][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][8][decoder_motion_interpolation] = false;
	mapping[12][8][decoder_motion_add] = false;
	mapping[12][8][decoder_motion_framebuf] = false;

	// Processor index 9
	mapping[12][9][decoder_merger] = false;
	mapping[12][9][decoder_parser_parseheaders] = false;
	mapping[12][9][decoder_parser_mvseq] = false;
	mapping[12][9][decoder_parser_blkexp] = false;
	mapping[12][9][decoder_parser_mvrecon] = false;
	mapping[12][9][decoder_texture_DCsplit] = false;
	mapping[12][9][decoder_texture_IS] = false;
	mapping[12][9][decoder_texture_IAP] = false;
	mapping[12][9][decoder_texture_IQ] = false;
	mapping[12][9][decoder_texture_idct2d] = false;
	mapping[12][9][decoder_texture_DCReconstruction_addressing] = true;
	mapping[12][9][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][9][decoder_motion_interpolation] = false;
	mapping[12][9][decoder_motion_add] = false;
	mapping[12][9][decoder_motion_framebuf] = false;

	// Processor index 10
	mapping[12][10][decoder_merger] = false;
	mapping[12][10][decoder_parser_parseheaders] = false;
	mapping[12][10][decoder_parser_mvseq] = false;
	mapping[12][10][decoder_parser_blkexp] = false;
	mapping[12][10][decoder_parser_mvrecon] = false;
	mapping[12][10][decoder_texture_DCsplit] = false;
	mapping[12][10][decoder_texture_IS] = false;
	mapping[12][10][decoder_texture_IAP] = false;
	mapping[12][10][decoder_texture_IQ] = false;
	mapping[12][10][decoder_texture_idct2d] = false;
	mapping[12][10][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][10][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][10][decoder_motion_interpolation] = true;
	mapping[12][10][decoder_motion_add] = false;
	mapping[12][10][decoder_motion_framebuf] = false;

	// Processor index 11
	mapping[12][11][decoder_merger] = false;
	mapping[12][11][decoder_parser_parseheaders] = false;
	mapping[12][11][decoder_parser_mvseq] = false;
	mapping[12][11][decoder_parser_blkexp] = false;
	mapping[12][11][decoder_parser_mvrecon] = false;
	mapping[12][11][decoder_texture_DCsplit] = false;
	mapping[12][11][decoder_texture_IS] = false;
	mapping[12][11][decoder_texture_IAP] = false;
	mapping[12][11][decoder_texture_IQ] = false;
	mapping[12][11][decoder_texture_idct2d] = false;
	mapping[12][11][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][11][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][11][decoder_motion_interpolation] = false;
	mapping[12][11][decoder_motion_add] = true;
	mapping[12][11][decoder_motion_framebuf] = false;

	// Processor index 12
	mapping[12][12][decoder_merger] = false;
	mapping[12][12][decoder_parser_parseheaders] = false;
	mapping[12][12][decoder_parser_mvseq] = false;
	mapping[12][12][decoder_parser_blkexp] = false;
	mapping[12][12][decoder_parser_mvrecon] = false;
	mapping[12][12][decoder_texture_DCsplit] = false;
	mapping[12][12][decoder_texture_IS] = false;
	mapping[12][12][decoder_texture_IAP] = false;
	mapping[12][12][decoder_texture_IQ] = false;
	mapping[12][12][decoder_texture_idct2d] = false;
	mapping[12][12][decoder_texture_DCReconstruction_addressing] = false;
	mapping[12][12][decoder_texture_DCReconstruction_invpred] = false;
	mapping[12][12][decoder_motion_interpolation] = false;
	mapping[12][12][decoder_motion_add] = false;
	mapping[12][12][decoder_motion_framebuf] = true;
}

static void setMapping_15(){	// Mapping index 15, 16 processors.

	// Processor index 0
	mapping[15][0][decoder_merger] = false;
	mapping[15][0][decoder_parser_parseheaders] = false;
	mapping[15][0][decoder_parser_mvseq] = false;
	mapping[15][0][decoder_parser_blkexp] = false;
	mapping[15][0][decoder_parser_mvrecon] = false;
	mapping[15][0][decoder_texture_DCsplit] = false;
	mapping[15][0][decoder_texture_IS] = false;
	mapping[15][0][decoder_texture_IAP] = false;
	mapping[15][0][decoder_texture_IQ] = false;
	mapping[15][0][decoder_texture_idct2d] = false;
	mapping[15][0][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][0][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][0][decoder_motion_interpolation] = false;
	mapping[15][0][decoder_motion_add] = false;
	mapping[15][0][decoder_motion_framebuf] = false;

	// Processor index 1
	mapping[15][1][decoder_merger] = true;
	mapping[15][1][decoder_parser_parseheaders] = false;
	mapping[15][1][decoder_parser_mvseq] = false;
	mapping[15][1][decoder_parser_blkexp] = false;
	mapping[15][1][decoder_parser_mvrecon] = false;
	mapping[15][1][decoder_texture_DCsplit] = false;
	mapping[15][1][decoder_texture_IS] = false;
	mapping[15][1][decoder_texture_IAP] = false;
	mapping[15][1][decoder_texture_IQ] = false;
	mapping[15][1][decoder_texture_idct2d] = false;
	mapping[15][1][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][1][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][1][decoder_motion_interpolation] = false;
	mapping[15][1][decoder_motion_add] = false;
	mapping[15][1][decoder_motion_framebuf] = false;

	// Processor index 2
	mapping[15][2][decoder_merger] = false;
	mapping[15][2][decoder_parser_parseheaders] = true;
	mapping[15][2][decoder_parser_mvseq] = false;
	mapping[15][2][decoder_parser_blkexp] = false;
	mapping[15][2][decoder_parser_mvrecon] = false;
	mapping[15][2][decoder_texture_DCsplit] = false;
	mapping[15][2][decoder_texture_IS] = false;
	mapping[15][2][decoder_texture_IAP] = false;
	mapping[15][2][decoder_texture_IQ] = false;
	mapping[15][2][decoder_texture_idct2d] = false;
	mapping[15][2][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][2][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][2][decoder_motion_interpolation] = false;
	mapping[15][2][decoder_motion_add] = false;
	mapping[15][2][decoder_motion_framebuf] = false;

	// Processor index 3
	mapping[15][3][decoder_merger] = false;
	mapping[15][3][decoder_parser_parseheaders] = false;
	mapping[15][3][decoder_parser_mvseq] = true;
	mapping[15][3][decoder_parser_blkexp] = false;
	mapping[15][3][decoder_parser_mvrecon] = false;
	mapping[15][3][decoder_texture_DCsplit] = false;
	mapping[15][3][decoder_texture_IS] = false;
	mapping[15][3][decoder_texture_IAP] = false;
	mapping[15][3][decoder_texture_IQ] = false;
	mapping[15][3][decoder_texture_idct2d] = false;
	mapping[15][3][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][3][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][3][decoder_motion_interpolation] = false;
	mapping[15][3][decoder_motion_add] = false;
	mapping[15][3][decoder_motion_framebuf] = false;

	// Processor index 4
	mapping[15][4][decoder_merger] = false;
	mapping[15][4][decoder_parser_parseheaders] = false;
	mapping[15][4][decoder_parser_mvseq] = false;
	mapping[15][4][decoder_parser_blkexp] = true;
	mapping[15][4][decoder_parser_mvrecon] = false;
	mapping[15][4][decoder_texture_DCsplit] = false;
	mapping[15][4][decoder_texture_IS] = false;
	mapping[15][4][decoder_texture_IAP] = false;
	mapping[15][4][decoder_texture_IQ] = false;
	mapping[15][4][decoder_texture_idct2d] = false;
	mapping[15][4][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][4][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][4][decoder_motion_interpolation] = false;
	mapping[15][4][decoder_motion_add] = false;
	mapping[15][4][decoder_motion_framebuf] = false;

	// Processor index 5
	mapping[15][5][decoder_merger] = false;
	mapping[15][5][decoder_parser_parseheaders] = false;
	mapping[15][5][decoder_parser_mvseq] = false;
	mapping[15][5][decoder_parser_blkexp] = false;
	mapping[15][5][decoder_parser_mvrecon] = true;
	mapping[15][5][decoder_texture_DCsplit] = false;
	mapping[15][5][decoder_texture_IS] = false;
	mapping[15][5][decoder_texture_IAP] = false;
	mapping[15][5][decoder_texture_IQ] = false;
	mapping[15][5][decoder_texture_idct2d] = false;
	mapping[15][5][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][5][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][5][decoder_motion_interpolation] = false;
	mapping[15][5][decoder_motion_add] = false;
	mapping[15][5][decoder_motion_framebuf] = false;

	// Processor index 6
	mapping[15][6][decoder_merger] = false;
	mapping[15][6][decoder_parser_parseheaders] = false;
	mapping[15][6][decoder_parser_mvseq] = false;
	mapping[15][6][decoder_parser_blkexp] = false;
	mapping[15][6][decoder_parser_mvrecon] = false;
	mapping[15][6][decoder_texture_DCsplit] = true;
	mapping[15][6][decoder_texture_IS] = false;
	mapping[15][6][decoder_texture_IAP] = false;
	mapping[15][6][decoder_texture_IQ] = false;
	mapping[15][6][decoder_texture_idct2d] = false;
	mapping[15][6][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][6][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][6][decoder_motion_interpolation] = false;
	mapping[15][6][decoder_motion_add] = false;
	mapping[15][6][decoder_motion_framebuf] = false;

	// Processor index 7
	mapping[15][7][decoder_merger] = false;
	mapping[15][7][decoder_parser_parseheaders] = false;
	mapping[15][7][decoder_parser_mvseq] = false;
	mapping[15][7][decoder_parser_blkexp] = false;
	mapping[15][7][decoder_parser_mvrecon] = false;
	mapping[15][7][decoder_texture_DCsplit] = false;
	mapping[15][7][decoder_texture_IS] = true;
	mapping[15][7][decoder_texture_IAP] = false;
	mapping[15][7][decoder_texture_IQ] = false;
	mapping[15][7][decoder_texture_idct2d] = false;
	mapping[15][7][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][7][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][7][decoder_motion_interpolation] = false;
	mapping[15][7][decoder_motion_add] = false;
	mapping[15][7][decoder_motion_framebuf] = false;

	// Processor index 8
	mapping[15][8][decoder_merger] = false;
	mapping[15][8][decoder_parser_parseheaders] = false;
	mapping[15][8][decoder_parser_mvseq] = false;
	mapping[15][8][decoder_parser_blkexp] = false;
	mapping[15][8][decoder_parser_mvrecon] = false;
	mapping[15][8][decoder_texture_DCsplit] = false;
	mapping[15][8][decoder_texture_IS] = false;
	mapping[15][8][decoder_texture_IAP] = true;
	mapping[15][8][decoder_texture_IQ] = false;
	mapping[15][8][decoder_texture_idct2d] = false;
	mapping[15][8][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][8][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][8][decoder_motion_interpolation] = false;
	mapping[15][8][decoder_motion_add] = false;
	mapping[15][8][decoder_motion_framebuf] = false;

	// Processor index 9
	mapping[15][9][decoder_merger] = false;
	mapping[15][9][decoder_parser_parseheaders] = false;
	mapping[15][9][decoder_parser_mvseq] = false;
	mapping[15][9][decoder_parser_blkexp] = false;
	mapping[15][9][decoder_parser_mvrecon] = false;
	mapping[15][9][decoder_texture_DCsplit] = false;
	mapping[15][9][decoder_texture_IS] = false;
	mapping[15][9][decoder_texture_IAP] = false;
	mapping[15][9][decoder_texture_IQ] = true;
	mapping[15][9][decoder_texture_idct2d] = false;
	mapping[15][9][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][9][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][9][decoder_motion_interpolation] = false;
	mapping[15][9][decoder_motion_add] = false;
	mapping[15][9][decoder_motion_framebuf] = false;

	// Processor index 10
	mapping[15][10][decoder_merger] = false;
	mapping[15][10][decoder_parser_parseheaders] = false;
	mapping[15][10][decoder_parser_mvseq] = false;
	mapping[15][10][decoder_parser_blkexp] = false;
	mapping[15][10][decoder_parser_mvrecon] = false;
	mapping[15][10][decoder_texture_DCsplit] = false;
	mapping[15][10][decoder_texture_IS] = false;
	mapping[15][10][decoder_texture_IAP] = false;
	mapping[15][10][decoder_texture_IQ] = false;
	mapping[15][10][decoder_texture_idct2d] = true;
	mapping[15][10][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][10][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][10][decoder_motion_interpolation] = false;
	mapping[15][10][decoder_motion_add] = false;
	mapping[15][10][decoder_motion_framebuf] = false;

	// Processor index 11
	mapping[15][11][decoder_merger] = false;
	mapping[15][11][decoder_parser_parseheaders] = false;
	mapping[15][11][decoder_parser_mvseq] = false;
	mapping[15][11][decoder_parser_blkexp] = false;
	mapping[15][11][decoder_parser_mvrecon] = false;
	mapping[15][11][decoder_texture_DCsplit] = false;
	mapping[15][11][decoder_texture_IS] = false;
	mapping[15][11][decoder_texture_IAP] = false;
	mapping[15][11][decoder_texture_IQ] = false;
	mapping[15][11][decoder_texture_idct2d] = false;
	mapping[15][11][decoder_texture_DCReconstruction_addressing] = true;
	mapping[15][11][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][11][decoder_motion_interpolation] = false;
	mapping[15][11][decoder_motion_add] = false;
	mapping[15][11][decoder_motion_framebuf] = false;

	// Processor index 12
	mapping[15][12][decoder_merger] = false;
	mapping[15][12][decoder_parser_parseheaders] = false;
	mapping[15][12][decoder_parser_mvseq] = false;
	mapping[15][12][decoder_parser_blkexp] = false;
	mapping[15][12][decoder_parser_mvrecon] = false;
	mapping[15][12][decoder_texture_DCsplit] = false;
	mapping[15][12][decoder_texture_IS] = false;
	mapping[15][12][decoder_texture_IAP] = false;
	mapping[15][12][decoder_texture_IQ] = false;
	mapping[15][12][decoder_texture_idct2d] = false;
	mapping[15][12][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][12][decoder_texture_DCReconstruction_invpred] = true;
	mapping[15][12][decoder_motion_interpolation] = false;
	mapping[15][12][decoder_motion_add] = false;
	mapping[15][12][decoder_motion_framebuf] = false;

	// Processor index 13
	mapping[15][13][decoder_merger] = false;
	mapping[15][13][decoder_parser_parseheaders] = false;
	mapping[15][13][decoder_parser_mvseq] = false;
	mapping[15][13][decoder_parser_blkexp] = false;
	mapping[15][13][decoder_parser_mvrecon] = false;
	mapping[15][13][decoder_texture_DCsplit] = false;
	mapping[15][13][decoder_texture_IS] = false;
	mapping[15][13][decoder_texture_IAP] = false;
	mapping[15][13][decoder_texture_IQ] = false;
	mapping[15][13][decoder_texture_idct2d] = false;
	mapping[15][13][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][13][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][13][decoder_motion_interpolation] = true;
	mapping[15][13][decoder_motion_add] = false;
	mapping[15][13][decoder_motion_framebuf] = false;

	// Processor index 14
	mapping[15][14][decoder_merger] = false;
	mapping[15][14][decoder_parser_parseheaders] = false;
	mapping[15][14][decoder_parser_mvseq] = false;
	mapping[15][14][decoder_parser_blkexp] = false;
	mapping[15][14][decoder_parser_mvrecon] = false;
	mapping[15][14][decoder_texture_DCsplit] = false;
	mapping[15][14][decoder_texture_IS] = false;
	mapping[15][14][decoder_texture_IAP] = false;
	mapping[15][14][decoder_texture_IQ] = false;
	mapping[15][14][decoder_texture_idct2d] = false;
	mapping[15][14][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][14][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][14][decoder_motion_interpolation] = false;
	mapping[15][14][decoder_motion_add] = true;
	mapping[15][14][decoder_motion_framebuf] = false;

	// Processor index 15
	mapping[15][15][decoder_merger] = false;
	mapping[15][15][decoder_parser_parseheaders] = false;
	mapping[15][15][decoder_parser_mvseq] = false;
	mapping[15][15][decoder_parser_blkexp] = false;
	mapping[15][15][decoder_parser_mvrecon] = false;
	mapping[15][15][decoder_texture_DCsplit] = false;
	mapping[15][15][decoder_texture_IS] = false;
	mapping[15][15][decoder_texture_IAP] = false;
	mapping[15][15][decoder_texture_IQ] = false;
	mapping[15][15][decoder_texture_idct2d] = false;
	mapping[15][15][decoder_texture_DCReconstruction_addressing] = false;
	mapping[15][15][decoder_texture_DCReconstruction_invpred] = false;
	mapping[15][15][decoder_motion_interpolation] = false;
	mapping[15][15][decoder_motion_add] = false;
	mapping[15][15][decoder_motion_framebuf] = true;
}

void init_mappings(){
	setMapping_00();
	setMapping_01();
	setMapping_02();
	setMapping_03();
	setMapping_04();
	setMapping_07();
	setMapping_11();
	setMapping_12();
	setMapping_15();
}

void dynamicMapping(int nbProcessors){
	int i, j;
	int tempSchedCaller[NB_ACTORS];
	int cntr = 0;
	for (i = 0; i < nbProcessors; i++) {
		cntr = 0;
		for (j = 0; j < NB_ACTORS; j++) {
			if(mapping[nbProcessors - 1][i][j] == true){
//				tempSchedCaller[cntr] = schedCallerAddr[j];
				tempSchedCaller[cntr] = actors[j];
				cntr++;
			}
		}
		if (cntr > 0){
			sendCtrlMsgType_blocking(&ctrl_fifo_output[i], MSG_ACTORS_MAP);
			push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output[i], (u8*)tempSchedCaller, cntr * sizeof(int));
		}
	}
}

u8 changeMapping(u32* prevNbProc, u32* currNbProc)
{
	char receivedChar = 0;
	while(!XUartPs_IsReceiveData(XPS_UART1_BASEADDR));
	receivedChar = inbyte();
//		if(((receivedChar == 0x31) || (receivedChar == 0x32) || (receivedChar == 0x34) || (receivedChar == 0x38) || (receivedChar == 0x40)) && ((receivedChar - 0x30) != currNbProc)){
	*prevNbProc = *currNbProc;
	switch (receivedChar - 0x30) {
		case 0:
			*currNbProc = 1;
			break;
		case 1:
			*currNbProc = 2;
			break;
		case 2:
			*currNbProc = 4;
			break;
		case 3:
			*currNbProc = 8;
			break;
		case 4:
			*currNbProc = 16;
			break;
	}
	if(*currNbProc != *prevNbProc)
//			currNbProc = receivedChar - 0x30;
//			currNbProc = 2 << (receivedChar - 0x30);
			return 1;
	else
		return 0;
}


void loadBalancing(){

}
