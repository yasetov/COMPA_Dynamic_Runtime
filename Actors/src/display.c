
// Source file is "L/RVC/src/org/sc29/wg11/common/DisplayYUV.cal"

#include <stdio.h>
#include <stdlib.h>
#include "dataflow.h"
#include "config.h"
#include "fifoAllocations.h"

//extern void xil_printf( const char *ctrl1, ...);

u32 frameCtr 	= 0;



////////////////////////////////////////////////////////////////////////////////
// Input FIFOs
//extern  fifo_u8_t *display_B;
//#define display_B_ix  1
//extern int readCmpInputTokens(u32 fifoIx, u8* data, u32 start, u32 end, u8 tokenSize);
//extern  fifo_i16_t *display_WIDTH;
//#define display_WIDTH_ix  6
//extern int readCmpInputTokens(u32 fifoIx, u8* data, u32 start, u32 end, u8 tokenSize);
//extern  fifo_i16_t *display_HEIGHT;
//#define display_HEIGHT_ix  4
//extern int readCmpInputTokens(u32 fifoIx, u8* data, u32 start, u32 end, u8 tokenSize);

////////////////////////////////////////////////////////////////////////////////
// Input Fifo control variables
static unsigned int index_B;
static unsigned int numTokens_B;
#define SIZE_B 512
#define tokens_B 	display_B->contents


static unsigned int index_WIDTH;
static unsigned int numTokens_WIDTH;
#define SIZE_WIDTH 512
#define tokens_WIDTH 	display_WIDTH->contents


static unsigned int index_HEIGHT;
static unsigned int numTokens_HEIGHT;
#define SIZE_HEIGHT 512
#define tokens_HEIGHT 	display_HEIGHT->contents



////////////////////////////////////////////////////////////////////////////////
// State variables of the actor

#define Display_MB_SIZE_LUMA 16
#define Display_MB_SIZE_CHROMA 8
#define Display_DISP_ENABLE 1
#define IMG_BUFF_ADDR			0x5a050 // image's length is 38016 (0x9480) bytes

#define IMG_WIDTH		176
#define IMG_HEIGHT		144
#define IMG_LENGTH_Y	IMG_WIDTH * IMG_HEIGHT
#define IMG_LENGTH_U	(IMG_WIDTH/2) * (IMG_HEIGHT/2) //4:2:0 coding
#define IMG_LENGTH 		(IMG_LENGTH_Y + (2*IMG_LENGTH_U))

//static u8 pictureBufferY[8388608];
//static u8 pictureBufferU[2097152];
//static u8 pictureBufferV[2097152];
//static u8 pictureBufferY[IMG_LENGTH_Y];
static u8* pictureBufferY = (u8*)IMG_BUFF_ADDR;
//static u8 pictureBufferU[IMG_LENGTH_U];
static u8* pictureBufferU = (u8*)IMG_BUFF_ADDR + IMG_LENGTH_Y;
//static u8 pictureBufferV[IMG_LENGTH_U];
static u8* pictureBufferV = (u8*)IMG_BUFF_ADDR + IMG_LENGTH_Y + IMG_LENGTH_U;

static i32 pictureSizeInMb;
static i32 nbBlockGot;
static i32 nbFrameDecoded;
static u8 chromaComponent;
//static i16 pictureWidthLuma = 4096;
//static i16 pictureHeightLuma = 2048;
static i16 pictureWidthLuma = IMG_WIDTH;
static i16 pictureHeightLuma = IMG_HEIGHT;
static u16 xIdxLuma;
static u32 yOffLuma;
static u16 xIdxChroma;
static u32 yOffChroma;
static i32 isTerminated = 0;

static u8 init = 0;

////////////////////////////////////////////////////////////////////////////////
// Initial FSM state of the actor
enum states {
	my_state_GetChroma1Block,
	my_state_GetChroma2Block,
	my_state_GetLumaBlock,
	my_state_GetPictureSize
};

static char *stateNames[] = {
	"GetChroma1Block",
	"GetChroma2Block",
	"GetLumaBlock",
	"GetPictureSize"
};

static enum states _FSM_state;

////////////////////////////////////////////////////////////////////////////////
// Token functions
static void read_B() {
	index_B = display_B->read_inds[0];
	numTokens_B = index_B + fifo_u8_get_num_tokens(display_B, 0);
}

static void read_end_B() {
	display_B->read_inds[0] = index_B;
}
static void read_WIDTH() {
	index_WIDTH = display_WIDTH->read_inds[0];
	numTokens_WIDTH = index_WIDTH + fifo_i16_get_num_tokens(display_WIDTH, 0);
}

static void read_end_WIDTH() {
	display_WIDTH->read_inds[0] = index_WIDTH;
}
static void read_HEIGHT() {
	index_HEIGHT = display_HEIGHT->read_inds[0];
	numTokens_HEIGHT = index_HEIGHT + fifo_i16_get_num_tokens(display_HEIGHT, 0);
}

static void read_end_HEIGHT() {
	display_HEIGHT->read_inds[0] = index_HEIGHT;
}


////////////////////////////////////////////////////////////////////////////////
// Functions/procedures
extern i32 source_isMaxLoopsReached();
extern i32 displayYUV_getNbFrames();
extern void source_exit(i32 exitCode);
extern u8 displayYUV_getFlags();
extern void fpsPrintNewPicDecoded();
extern void displayYUV_displayPicture(u8 pictureBufferY[8388608], u8 pictureBufferU[8388608], u8 pictureBufferV[8388608], i16 pictureWidth, i16 pictureSize);
extern void compareYUV_comparePicture(u8 pictureBufferY[8388608], u8 pictureBufferU[8388608], u8 pictureBufferV[8388608], i16 pictureWidth, i16 pictureSize);
extern void displayYUV_init();
extern void compareYUV_init();
extern void fpsPrintInit();


////////////////////////////////////////////////////////////////////////////////
// Actions
static i32 isSchedulable_untagged_0() {
	i32 result;
	i32 local_isTerminated;
	i32 tmp_source_isMaxLoopsReached;
	i32 local_nbFrameDecoded;
	i32 tmp_displayYUV_getNbFrames;

	local_isTerminated = isTerminated;
//	tmp_source_isMaxLoopsReached = source_isMaxLoopsReached();
	local_nbFrameDecoded = nbFrameDecoded;
	tmp_displayYUV_getNbFrames = displayYUV_getNbFrames();
	result = !local_isTerminated && (tmp_source_isMaxLoopsReached || local_nbFrameDecoded == tmp_displayYUV_getNbFrames);
	return result;
}

static void untagged_0() {

	// Compute aligned port indexes


	isTerminated = 1;
//	source_exit(0);

	// Update ports indexes

}
static i32 isSchedulable_getPictureSize() {
	i32 result;

	result = 1;
	return result;
}

static void getPictureSize() {

	// Compute aligned port indexes
	i32 index_aligned_WIDTH = index_WIDTH % SIZE_WIDTH;
	i32 index_aligned_HEIGHT = index_HEIGHT % SIZE_HEIGHT;

	i16 WidthValue;
	i16 HeightValue;

	WidthValue = tokens_WIDTH[(index_WIDTH + (0)) % SIZE_WIDTH];
	HeightValue = tokens_HEIGHT[(index_HEIGHT + (0)) % SIZE_HEIGHT];
	pictureWidthLuma = WidthValue * 16;
	pictureHeightLuma = HeightValue * 16;
	pictureSizeInMb = WidthValue * HeightValue;
	nbBlockGot = 0;
	xIdxLuma = 0;
	xIdxChroma = 0;
	yOffLuma = 0;
	yOffChroma = 0;

	// Update ports indexes
	index_WIDTH += 1;
	index_HEIGHT += 1;

	read_end_WIDTH();
	read_end_HEIGHT();
}
static i32 isSchedulable_getPixValue_launch_Luma() {
	i32 result;
	i32 local_nbBlockGot;
	i32 local_pictureSizeInMb;

	local_nbBlockGot = nbBlockGot;
	local_pictureSizeInMb = pictureSizeInMb;
	result = local_nbBlockGot < local_pictureSizeInMb;
	return result;
}

static void getPixValue_launch_Luma() {

	// Compute aligned port indexes
	i32 index_aligned_B = index_B % SIZE_B;

	u32 local_yOffLuma;
	u32 yOff;
	u16 idx;
	i32 local_nbBlockGot;
	i32 y;
	i32 x;
	u16 local_xIdxLuma;
	u8 tmp_B;
	i16 local_pictureWidthLuma;
	u32 local_MB_SIZE_LUMA;

	local_yOffLuma = yOffLuma;
	yOff = local_yOffLuma;
	idx = 0;
	local_nbBlockGot = nbBlockGot;
	nbBlockGot = local_nbBlockGot + 1;
	y = 0;
	while (y <= 15) {
		local_xIdxLuma = xIdxLuma;
		x = local_xIdxLuma;
		local_xIdxLuma = xIdxLuma;
		while (x <= local_xIdxLuma + 15) {
			tmp_B = tokens_B[(index_B + (idx)) % SIZE_B];
			pictureBufferY[yOff + x] = tmp_B;
			idx = idx + 1;
			x = x + 1;
		}
		local_pictureWidthLuma = pictureWidthLuma;
		yOff = yOff + local_pictureWidthLuma;
		y = y + 1;
	}
	local_xIdxLuma = xIdxLuma;
	local_MB_SIZE_LUMA = Display_MB_SIZE_LUMA;
	xIdxLuma = local_xIdxLuma + local_MB_SIZE_LUMA;
	chromaComponent = 0;

	// Update ports indexes
	index_B += 256;

	read_end_B();
}
static i32 isSchedulable_getPixValue_launch_Chroma() {
	i32 result;

	result = 1;
	return result;
}

static void getPixValue_launch_Chroma() {

	// Compute aligned port indexes
	i32 index_aligned_B = index_B % SIZE_B;

	u32 local_yOffChroma;
	u32 yOff;
	u8 idx;
	i32 y;
	i32 x;
	u16 local_xIdxChroma;
	u8 local_chromaComponent;
	u8 tmp_B;
	u8 tmp_B0;
	i16 local_pictureWidthLuma;
	u32 local_MB_SIZE_CHROMA;
	u16 local_xIdxLuma;
	u32 local_yOffLuma;
	u32 local_MB_SIZE_LUMA;

	local_yOffChroma = yOffChroma;
	yOff = local_yOffChroma;
	idx = 0;
	y = 0;
	while (y <= 7) {
		local_xIdxChroma = xIdxChroma;
		x = local_xIdxChroma;
		local_xIdxChroma = xIdxChroma;
		while (x <= local_xIdxChroma + 7) {
			local_chromaComponent = chromaComponent;
			if (local_chromaComponent == 0) {
				tmp_B = tokens_B[(index_B + (idx)) % SIZE_B];
				pictureBufferU[yOff + x] = tmp_B;
			} else {
				tmp_B0 = tokens_B[(index_B + (idx)) % SIZE_B];
				pictureBufferV[yOff + x] = tmp_B0;
			}
			idx = idx + 1;
			x = x + 1;
		}
		local_pictureWidthLuma = pictureWidthLuma;
		yOff = yOff + local_pictureWidthLuma / 2;
		y = y + 1;
	}
	local_chromaComponent = chromaComponent;
	if (local_chromaComponent != 0) {
		local_xIdxChroma = xIdxChroma;
		local_MB_SIZE_CHROMA = Display_MB_SIZE_CHROMA;
		xIdxChroma = local_xIdxChroma + local_MB_SIZE_CHROMA;
		local_xIdxLuma = xIdxLuma;
		local_pictureWidthLuma = pictureWidthLuma;
		if (local_xIdxLuma == local_pictureWidthLuma) {
			xIdxLuma = 0;
			xIdxChroma = 0;
			local_yOffLuma = yOffLuma;
			local_MB_SIZE_LUMA = Display_MB_SIZE_LUMA;
			local_pictureWidthLuma = pictureWidthLuma;
			yOffLuma = local_yOffLuma + local_MB_SIZE_LUMA * local_pictureWidthLuma;
			local_yOffChroma = yOffChroma;
			local_MB_SIZE_CHROMA = Display_MB_SIZE_CHROMA;
			local_pictureWidthLuma = pictureWidthLuma;
			yOffChroma = local_yOffChroma + local_MB_SIZE_CHROMA * local_pictureWidthLuma / 2;
		}
	}
	chromaComponent = 1;

	// Update ports indexes
	index_B += 64;

	read_end_B();
}
static i32 isSchedulable_displayPicture() {
	i32 result;
	i32 local_nbBlockGot;
	i32 local_pictureSizeInMb;
	u8 tmp_displayYUV_getFlags;
	u8 local_DISP_ENABLE;

	local_nbBlockGot = nbBlockGot;
	local_pictureSizeInMb = pictureSizeInMb;
	tmp_displayYUV_getFlags = displayYUV_getFlags();
	local_DISP_ENABLE = Display_DISP_ENABLE;
	result = local_nbBlockGot >= local_pictureSizeInMb && (tmp_displayYUV_getFlags & local_DISP_ENABLE) != 0;
	return result;
}

static void displayPicture() {

	// Compute aligned port indexes

	i16 local_pictureWidthLuma;
	i16 local_pictureHeightLuma;
	i32 local_nbFrameDecoded;

//	fpsPrintNewPicDecoded();
	local_pictureWidthLuma = pictureWidthLuma;
	local_pictureHeightLuma = pictureHeightLuma;
	displayYUV_displayPicture(pictureBufferY, pictureBufferU, pictureBufferV, local_pictureWidthLuma, local_pictureHeightLuma);
	local_pictureWidthLuma = pictureWidthLuma;
	local_pictureHeightLuma = pictureHeightLuma;
//	compareYUV_comparePicture(pictureBufferY, pictureBufferU, pictureBufferV, local_pictureWidthLuma, local_pictureHeightLuma);
	local_nbFrameDecoded = nbFrameDecoded;
	nbFrameDecoded = local_nbFrameDecoded + 1;

	// Update ports indexes

}
static i32 isSchedulable_displayDisable() {
	i32 result;
	i32 local_nbBlockGot;
	i32 local_pictureSizeInMb;

	local_nbBlockGot = nbBlockGot;
	local_pictureSizeInMb = pictureSizeInMb;
	result = local_nbBlockGot >= local_pictureSizeInMb;
	return result;
}

static void displayDisable() {

	// Compute aligned port indexes

	i16 local_pictureWidthLuma;
	i16 local_pictureHeightLuma;
	i32 local_nbFrameDecoded;

//	fpsPrintNewPicDecoded();
	local_pictureWidthLuma = pictureWidthLuma;
	local_pictureHeightLuma = pictureHeightLuma;
//	compareYUV_comparePicture(pictureBufferY, pictureBufferU, pictureBufferV, local_pictureWidthLuma, local_pictureHeightLuma);
	local_nbFrameDecoded = nbFrameDecoded;
	nbFrameDecoded = local_nbFrameDecoded + 1;

	// Update ports indexes

}

////////////////////////////////////////////////////////////////////////////////
// Initializes
static i32 isSchedulable_untagged_1() {
	i32 result;

	result = 1;
	return result;
}

static void untagged_1() {

	// Compute aligned port indexes

	u8 tmp_displayYUV_getFlags;
	u8 local_DISP_ENABLE;

	tmp_displayYUV_getFlags = displayYUV_getFlags();
	local_DISP_ENABLE = Display_DISP_ENABLE;
	if ((tmp_displayYUV_getFlags & local_DISP_ENABLE) != 0) {
		displayYUV_init();
	}
//	compareYUV_init();
//	fpsPrintInit();
	nbFrameDecoded = 0;

	// Update ports indexes

}

void display_initialize() {
	int i = 0;
	/* Set initial state to current FSM state */
	_FSM_state = my_state_GetPictureSize;
	// Action loop
	if (isSchedulable_untagged_1()) {
		int stop = 0;
		if (stop != 0) {
			goto finished;
		}
		untagged_1();
		i++;
	} else {
		goto finished;
	}
	
finished:
	// no read_end/write_end here!
	return;
}

////////////////////////////////////////////////////////////////////////////////
// Action scheduler
int display_outside_FSM_scheduler() {
	int i = 0;
	while (1) {
		// Action loop
		if (isSchedulable_untagged_0()) {
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

int display_scheduler() {
	int i = 0;
	if(!init){
		display_initialize();
		init = 1;
	}

	read_B();
	read_WIDTH();
	read_HEIGHT();

	u32 start_index_B = index_B;
	u32 start_index_HEIGHT = index_HEIGHT;
	u32 start_index_WIDTH = index_WIDTH;

	// jump to FSM state 
	switch (_FSM_state) {
	case my_state_GetChroma1Block:
		goto l_GetChroma1Block;
	case my_state_GetChroma2Block:
		goto l_GetChroma2Block;
	case my_state_GetLumaBlock:
		goto l_GetLumaBlock;
	case my_state_GetPictureSize:
		goto l_GetPictureSize;
	default:
//		xil_printf("unknown state in display.c : %s\n", stateNames[_FSM_state]);
		exit(1);
	}

	// FSM transitions
l_GetChroma1Block:
	i += display_outside_FSM_scheduler();
	if (numTokens_B - index_B >= 64 && isSchedulable_getPixValue_launch_Chroma()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetChroma1Block;
			goto finished;
		}
		getPixValue_launch_Chroma();
		i++;
		goto l_GetChroma2Block;
	} else {
		_FSM_state = my_state_GetChroma1Block;
		goto finished;
	}
l_GetChroma2Block:
	i += display_outside_FSM_scheduler();
	if (numTokens_B - index_B >= 64 && isSchedulable_getPixValue_launch_Chroma()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetChroma2Block;
			goto finished;
		}
		getPixValue_launch_Chroma();
		i++;
		goto l_GetLumaBlock;
	} else {
		_FSM_state = my_state_GetChroma2Block;
		goto finished;
	}
l_GetLumaBlock:
	i += display_outside_FSM_scheduler();
	if (numTokens_B - index_B >= 256 && isSchedulable_getPixValue_launch_Luma()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetLumaBlock;
			goto finished;
		}
		getPixValue_launch_Luma();
		i++;
		goto l_GetChroma1Block;
	} else if (isSchedulable_displayPicture()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetLumaBlock;
			goto finished;
		}
		displayPicture();
		frameCtr++;
//		xil_printf("%d\n", frameCtr);
		i++;
		goto l_GetPictureSize;
	} else if (isSchedulable_displayDisable()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetLumaBlock;
			goto finished;
		}
		displayDisable();
		i++;
		goto l_GetPictureSize;
	} else {
		_FSM_state = my_state_GetLumaBlock;
		goto finished;
	}
l_GetPictureSize:
	i += display_outside_FSM_scheduler();
	if (numTokens_WIDTH - index_WIDTH >= 1 && numTokens_HEIGHT - index_HEIGHT >= 1 && isSchedulable_getPictureSize()) {
		int stop = 0;
		if (stop != 0) {
			_FSM_state = my_state_GetPictureSize;
			goto finished;
		}
		getPictureSize();
		i++;
		goto l_GetLumaBlock;
	} else {
		_FSM_state = my_state_GetPictureSize;
		goto finished;
	}
finished:
//	if(i>0){	// At least one action was executed.
//		// Comparing input tokens with data copied into an extra buffer for debugging communications.
//		if(readCmpInputTokens(display_B_ix, (u8*)tokens_B, start_index_B, index_B, sizeof(u8))){
//			index_B = 0;
//			read_end_B();
//			exit(-1);
//		}
//		if(readCmpInputTokens(display_HEIGHT_ix, (i16*)tokens_HEIGHT, start_index_HEIGHT, index_HEIGHT, sizeof(i16))){
//			index_HEIGHT = 0;
//			read_end_HEIGHT();
//			exit(-1);
//		}
//		if(readCmpInputTokens(display_WIDTH_ix, (i16*)tokens_WIDTH, start_index_WIDTH, index_WIDTH, sizeof(i16))){
//			index_WIDTH = 0;
//			read_end_WIDTH();
//			exit(-1);
//		}
//	}

	read_end_B();
	read_end_WIDTH();
	read_end_HEIGHT();
	return i;
}
