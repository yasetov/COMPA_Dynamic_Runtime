/*
 * Copyright (c) 2009, IETR/INSA of Rennes
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of the IETR/INSA of Rennes nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

//#include <SDL.h>
#include "xil_io.h"
#include "config.h"
//#include "ctrl_fifos.h"

//#define VIDEO_BASEADDR		0x30000000
#define VIDEO_BASEADDR		0x50 // 640 * 144 * 4 = 368640 (5A000) bytes; screen width * image height * bytes per pixels.

//extern u8* bytesRead;
//extern ctrl_fifo_hdlr ctrl_fifo_output;

//static SDL_Surface *m_screen;
//static SDL_Surface *m_image;
//static SDL_Overlay *m_overlay;

static int init = 0;

static int x, y , onclick = 0;
//static SDL_Rect rect;

char displayYUV_getFlags(){
//	return display_flags;
}
//
//static void displayYUV_setSize(int width, int height) {
//    print_orcc_trace(ORCC_VL_VERBOSE_1, "set display to %ix%i", width, height);
//
//	m_screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
//	if (m_screen == NULL) {
//		fprintf(stderr, "Couldn't set %ix%ix24 video mode: %s\n", width,
//                height, SDL_GetError());
//	}
//
//	if (m_overlay != NULL) {
//		SDL_FreeYUVOverlay(m_overlay);
//	}
//
//	m_overlay = SDL_CreateYUVOverlay(width, height, SDL_YV12_OVERLAY, m_screen);
//	if (m_overlay == NULL) {
//        fprintf(stderr, "Couldn't create overlay: %s\n", SDL_GetError());
//    }
//}

void DDRVideoWr(u8* pictureBuffer){
	unsigned long  pixel      = 0;
	unsigned short line       = 0;
	unsigned long  index_Y    = 0;
	unsigned long  index_V    = IMG_LENGTH_Y;
	unsigned long  index_U    = IMG_LENGTH_Y + IMG_LENGTH_U;
	unsigned long  first_line_index_U = index_U;
	unsigned long  first_line_index_V = index_V;
	unsigned short horizontalActiveTime = 640;
	unsigned short verticalActiveTime = 480;
	static unsigned int frmBuffAddr = VIDEO_BASEADDR;

	while(line < IMG_HEIGHT)
	{
		Xil_Out8(frmBuffAddr+(pixel*4), pictureBuffer[index_V]);
//		Xil_Out8(frmBuffAddr+(pixel*4), 0x80);
		Xil_Out8(frmBuffAddr+(pixel*4)+1, pictureBuffer[index_Y]);
		Xil_Out8(frmBuffAddr+(pixel*4)+2, pictureBuffer[index_U]);
//		Xil_Out8(frmBuffAddr+(pixel*4)+2, 0x80);
		Xil_Out8(frmBuffAddr+(pixel*4)+3, 0x00);
		index_Y++;

		if((pixel - line*horizontalActiveTime) < IMG_WIDTH-1)
		{
			pixel++;
			if(pixel % 2 == 0){
				index_U++; index_V++;
			}
		}
		else
		{
			line++;
			if(line == IMG_HEIGHT)
			{
//				microblaze_flush_dcache();
//				if(frmBuffAddr < VIDEO_BASEADDR + 2 * horizontalActiveTime * verticalActiveTime * 4)
//					frmBuffAddr += horizontalActiveTime * verticalActiveTime * 4;
//				else
//					frmBuffAddr = VIDEO_BASEADDR;
//				return;
			}
			pixel = line*horizontalActiveTime;
			if(line % 2 == 0){
				index_U++; index_V++;
				first_line_index_U = index_U;
				first_line_index_V = index_V;
			}
			else{
				index_U = first_line_index_U;
				index_V = first_line_index_V;
			}
		}
	}
//	Xil_DCacheFlush();
}

void displayYUV_displayPicture(unsigned char *pictureBufferY,
                               unsigned char *pictureBufferU,
                               unsigned char *pictureBufferV,
                               short   pictureWidth,
                               short   pictureHeight) {
	u32 StartCount;
	static unsigned int lastWidth = 0;
    static unsigned int lastHeight = 0;
//    SDL_Event event;

//	rect.w = pictureWidth;
//	rect.h = pictureHeight;

//	if ((pictureHeight != lastHeight) || (pictureWidth != lastWidth)) {
////		displayYUV_setSize(pictureWidth, pictureHeight);
//		lastHeight = pictureHeight;
//		lastWidth = pictureWidth;
//	}

//	if (SDL_LockYUVOverlay(m_overlay) < 0) {
//		fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
//        exit(-1);
//	}
//
//	memcpy(m_overlay->pixels[0], pictureBufferY, pictureWidth * pictureHeight);
//	memcpy(m_overlay->pixels[1], pictureBufferV, pictureWidth * pictureHeight / 4);
//	memcpy(m_overlay->pixels[2], pictureBufferU, pictureWidth * pictureHeight / 4);
//
//	SDL_UnlockYUVOverlay(m_overlay);
//    SDL_DisplayYUVOverlay(m_overlay, &rect);
//
//	/* Grab all the events off the queue. */
//	while (SDL_PollEvent(&event)) {
//		switch (event.type) {
//		case SDL_QUIT:
//			exit(0);
//			break;
//		default:
//			break;
//		}
//	}


//	Xil_ICacheEnable();
//	Xil_DCacheEnable();
//
//	StartCount = HAL_GetCurrentMsCount();

    microblaze_disable_dcache();
	DDRVideoWr(pictureBufferY);
	microblaze_enable_dcache();

//    while(IMG_LENGTH > ctrl_fifo_output.fifo.size - ctrl_fifo_output.fifo.write_ind[0] + ctrl_fifo_output.fifo.read_inds[0]);
//    ctrl_fifo_output.fifo.write_ind[0] += IMG_LENGTH;
}


void displayYUV_init() {

}

/**
 * @brief Return the number of frames the user want to decode before exiting the application.
 * If user didn't use the -f flag, it returns -1 (DEFAULT_INFINITEà).
 * @return The
 */
int displayYUV_getNbFrames() {
//	return nbFrames;
}

void display_close() {
//	SDL_Quit();
}
