/*
 * ctrlComms.h
 *
 *  Created on: 23 janv. 2015
 *      Author: yolivave
 */

#ifndef CTRLCOMMS_H_
#define CTRLCOMMS_H_

#include "config.h"
#include "ctrl_fifo_functions.h"

extern ctrl_fifo_hdlr ctrl_fifo_output[NB_PROCESSORS];		/* Control fifos*/
extern ctrl_fifo_hdlr ctrl_fifo_input[NB_PROCESSORS];

#endif /* CTRLCOMMS_H_ */
