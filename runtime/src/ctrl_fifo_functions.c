/*
 * ctrl_fifo_functions.c
 *
 *  Created on: 15 déc. 2014
 *      Author: yolivave
 */
#include "ctrl_fifo_functions.h"


void init_ctrl_fifo_hdlr(ctrl_fifo_hdlr* ctrl_fifo,
								u32 content_addr,
								u32 rd_ix_addr,
								u32 wr_ix_addr,
								u32 size){
	ctrl_fifo->fifo.contents = (u8*)content_addr;
	ctrl_fifo->fifo.read_inds = (u32*)rd_ix_addr;
	ctrl_fifo->fifo.readers_nb = 1;
	ctrl_fifo->fifo.size = size;
	ctrl_fifo->fifo.write_ind = (u32*)wr_ix_addr;
//	{CTRL_FIFO_SIZE, (u8 *) , 1, (u32 *) , (u32 *) };
}

//void rd_ctrl_fifo_indices(ctrl_fifo_hdlr* ctrl_fifo) {
//	ctrl_fifo->ctrl_fifo_num_tkn = ctrl_fifo->ctrl_fifo_rd_ix + fifo_u8_get_num_tokens(&ctrl_fifo->fifo, 0);
//	ctrl_fifo->ctrl_fifo_num_free = ctrl_fifo->ctrl_fifo_wr_ix + fifo_u8_get_room(&ctrl_fifo->fifo, 1);
//	ctrl_fifo->ctrl_fifo_rd_ix = ctrl_fifo->fifo.read_inds[0];
//	ctrl_fifo->ctrl_fifo_wr_ix = ctrl_fifo->fifo.write_ind[0];
//}

u32 rd_input_ctrl_fifo_nb_elements(ctrl_fifo_hdlr* ctrl_fifo){
//	ctrl_fifo->ctrl_fifo_num_elmnts = ctrl_fifo->fifo.read_inds[0] + fifo_u8_get_num_tokens(&ctrl_fifo->fifo, 0);
//	ctrl_fifo->ctrl_fifo_num_elmnts = fifo_u8_get_num_tokens(&ctrl_fifo->fifo, 0);
	ctrl_fifo->ctrl_fifo_wr_ix = ctrl_fifo->fifo.write_ind[0];
	ctrl_fifo->ctrl_fifo_rd_ix = ctrl_fifo->fifo.read_inds[0];
	ctrl_fifo->ctrl_fifo_num_elmnts = ctrl_fifo->ctrl_fifo_wr_ix - ctrl_fifo->ctrl_fifo_rd_ix;
	return ctrl_fifo->ctrl_fifo_num_elmnts;
}

u32 rd_output_ctrl_fifo_nb_free(ctrl_fifo_hdlr* ctrl_fifo){
//	ctrl_fifo->ctrl_fifo_num_free = ctrl_fifo->fifo.write_ind[0] + fifo_u8_get_room(&ctrl_fifo->fifo, 1);
//	ctrl_fifo->ctrl_fifo_num_free = fifo_u8_get_room(&ctrl_fifo->fifo, 1);
	ctrl_fifo->ctrl_fifo_rd_ix = ctrl_fifo->fifo.read_inds[0];
	ctrl_fifo->ctrl_fifo_wr_ix = ctrl_fifo->fifo.write_ind[0];
	ctrl_fifo->ctrl_fifo_num_free = ctrl_fifo->fifo.size - (ctrl_fifo->ctrl_fifo_wr_ix - ctrl_fifo->ctrl_fifo_rd_ix);
	return ctrl_fifo->ctrl_fifo_num_free;
}


//u8* pop_input_ctrl_fifo_contents(ctrl_fifo_hdlr* ctrl_fifo){
//	u32 rd_ix = ctrl_fifo->fifo.read_inds[0];
//	ctrl_fifo->fifo.read_inds[0] = ctrl_fifo->fifo.write_ind[0];
//	return &ctrl_fifo->fifo.contents[rd_ix];
//}


u32 pop_contents_input_ctrl_fifo_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8* dst){
	u32 i;
	while(!rd_input_ctrl_fifo_nb_elements(ctrl_fifo));
	for (i = 0; i < ctrl_fifo->ctrl_fifo_num_elmnts; i++) {
		dst[i] = ctrl_fifo->fifo.contents[(ctrl_fifo->ctrl_fifo_rd_ix + i) % ctrl_fifo->fifo.size];
	}
	ctrl_fifo->fifo.read_inds[0] = ctrl_fifo->fifo.write_ind[0];

	return ctrl_fifo->ctrl_fifo_num_elmnts;
}

u32 pop_contents_input_ctrl_fifo_extended_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8* dst, int nbBytes){
	u32 i;
	while(rd_input_ctrl_fifo_nb_elements(ctrl_fifo) < nbBytes);
	for (i = 0; i < nbBytes; i++) {
		dst[i] = ctrl_fifo->fifo.contents[(ctrl_fifo->ctrl_fifo_rd_ix + i ) % ctrl_fifo->fifo.size];
	}
	ctrl_fifo->fifo.read_inds[0] += nbBytes;

	return nbBytes;
}

void push_contents_output_ctrl_fifo_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8* data, int nbElements){
	int i;
	while(rd_output_ctrl_fifo_nb_free(ctrl_fifo) < nbElements);
	for (i = 0; i < nbElements; i++) {
		ctrl_fifo->fifo.contents[(ctrl_fifo->ctrl_fifo_wr_ix + i) % ctrl_fifo->fifo.size] = data[i];
	}
	ctrl_fifo->fifo.write_ind[0] += nbElements;
}

void sendCtrlMsgType(ctrl_fifo_hdlr* ctrl_fifo, u8 msg){
	if(rd_output_ctrl_fifo_nb_free(ctrl_fifo) >= sizeof(msg)){
		ctrl_fifo->fifo.contents[ctrl_fifo->ctrl_fifo_wr_ix % ctrl_fifo->fifo.size] = msg;
		ctrl_fifo->fifo.write_ind[0] += sizeof(msg);
	}
}

void sendCtrlMsgType_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8 msg){
	while(rd_output_ctrl_fifo_nb_free(ctrl_fifo) <= sizeof(msg));
	ctrl_fifo->fifo.contents[ctrl_fifo->ctrl_fifo_wr_ix % ctrl_fifo->fifo.size] = msg;
	ctrl_fifo->fifo.write_ind[0] += sizeof(msg);
}

void sendCtrlMsg_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8 msg, u8* data, int nbBytes){
	int i;
	int msgSz = sizeof(msg) + nbBytes;
	switch (msg) {
		case MSG_VERIFY_DATA_FIFOS:
			while(rd_output_ctrl_fifo_nb_free(ctrl_fifo) < msgSz);
			ctrl_fifo->fifo.contents[ctrl_fifo->ctrl_fifo_wr_ix % ctrl_fifo->fifo.size] = msg; // Copying message type
			ctrl_fifo->ctrl_fifo_wr_ix += sizeof(msg);

			for (i = 0; i < nbBytes; i++) { // Copying the message's payload.
				ctrl_fifo->fifo.contents[(ctrl_fifo->ctrl_fifo_wr_ix + i) % ctrl_fifo->fifo.size] = data[i];
			}
			break;
		default:
			break;
	}
	ctrl_fifo->fifo.write_ind[0] += msgSz;
}

u8 rcvCtrlMsgType_blocking(ctrl_fifo_hdlr* ctrl_fifo){
	u8 msg;
	while(!rd_input_ctrl_fifo_nb_elements(ctrl_fifo));
	msg = ctrl_fifo->fifo.contents[ctrl_fifo->ctrl_fifo_rd_ix % ctrl_fifo->fifo.size];
	ctrl_fifo->fifo.read_inds[0] += sizeof(u8);
	return msg;
}

u8 rcvCtrlMsgType(ctrl_fifo_hdlr* ctrl_fifo){
	u8 msg = MSG_NONE;
	if(rd_input_ctrl_fifo_nb_elements(ctrl_fifo)){
		msg = ctrl_fifo->fifo.contents[ctrl_fifo->ctrl_fifo_rd_ix % ctrl_fifo->fifo.size];
		ctrl_fifo->fifo.read_inds[0] += sizeof(u8);
	}
	return msg;

}
//
//void updt_rd_ix(ctrl_fifo_hdlr* ctrl_fifo) {
//	ctrl_fifo->fifo.read_inds[0] = ctrl_fifo->ctrl_fifo_rd_ix;
//}
//
//void updt_wr_ix(ctrl_fifo_hdlr* ctrl_fifo) {
//	ctrl_fifo->fifo.write_ind[0] = ctrl_fifo->ctrl_fifo_wr_ix;
//}
//
//void clear_rd_ix(ctrl_fifo_hdlr* ctrl_fifo){
//	ctrl_fifo->fifo.read_inds[0] = 0;
//}
