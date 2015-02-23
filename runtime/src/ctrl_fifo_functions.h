#ifndef CTRL_FIFO_FUNCTIONS
#define CTRL_FIFO_FUNCTIONS

#include "types.h"

struct fifo_u8_t {
	unsigned int size; /** size of the ringbuffer */
	u8 *contents; /** the memory containing the ringbuffer */

	unsigned int readers_nb; /** the number of fifo's readers */
	unsigned int* read_inds; /** the current position of the reader */
	unsigned int* write_ind; /** the current position of the writer */
};

// #define PRINT_FIRINGS

/*********************
 * Control messages. Defined as macros because the size of an enum depends on the compiler.
 */

#define MSG_NONE							0
#define	MSG_VERIFY_DATA_FIFOS				1
#define MSG_VERIFY_DATA_FIFOS_OK			2
#define	MSG_VERIFY_DATA_FIFOS_ERROR			3
#define MSG_ACTORS_MAP						4
#define MSG_FLUSH_DCACHE					5
#define MSG_FLUSH_DCACHE_DONE				6
#define MSG_INIT_DONE						7
#define MSG_GET_METRICS						8
#define MSG_GET_METRICS_OK					9

/************************/

typedef struct{
	unsigned int ctrl_fifo_rd_ix;
	unsigned int ctrl_fifo_wr_ix;
	unsigned int ctrl_fifo_num_elmnts;
	unsigned int ctrl_fifo_num_free;
	struct fifo_u8_t fifo;
}ctrl_fifo_hdlr;

void init_ctrl_fifo_hdlr(ctrl_fifo_hdlr* ctrl_fifo,
								unsigned int content_addr,
								unsigned int rd_ix_addr,
								unsigned int wr_ix_addr,
								unsigned int size);

u32 rd_input_ctrl_fifo_nb_elements(ctrl_fifo_hdlr* ctrl_fifo);

u32 rd_output_ctrl_fifo_nb_free(ctrl_fifo_hdlr* ctrl_fifo);

u32 copy_input_ctrl_fifo_contents(ctrl_fifo_hdlr* ctrl_fifo, u8* dst);

u32 copy_input_ctrl_fifo_contents_ex(ctrl_fifo_hdlr* ctrl_fifo, u8* dst, int nbBytes);

u32 pop_contents_input_ctrl_fifo_extended_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8* dst, int nbBytes);

void push_contents_output_ctrl_fifo_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8* data, int nbElements);

void sendCtrlMsgType(ctrl_fifo_hdlr* ctrl_fifo, u8 msg);

void sendCtrlMsgType_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8 msg);

void sendCtrlMsg_blocking(ctrl_fifo_hdlr* ctrl_fifo, u8 msg, u8* data, int nbBytes);

u8 rcvCtrlMsgType_blocking(ctrl_fifo_hdlr* ctrl_fifo);

u8 rcvCtrlMsgType(ctrl_fifo_hdlr* ctrl_fifo);

#endif //CTRL_FIFO_FUNCTIONS
