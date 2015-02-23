/*
 * main.c
 *
 *  Created on: 18 déc. 2014
 *      Author: yolivave
 */

/////////////////////////////////////////////////
// Action schedulers

//extern void call_decoder_merger_scheduler();
//extern void call_decoder_parser_parseheaders_scheduler();
//extern void call_decoder_parser_mvseq_scheduler();
//extern void call_decoder_parser_blkexp_scheduler();
//extern void call_decoder_parser_mvrecon_scheduler();
//extern void call_decoder_texture_DCsplit_scheduler();
//extern void call_decoder_texture_IS_scheduler();
//extern void call_decoder_texture_IAP_scheduler();
//extern void call_decoder_texture_IQ_scheduler();
//extern void call_decoder_texture_idct2d_scheduler();
//extern void call_decoder_texture_DCReconstruction_addressing_scheduler();
//extern void call_decoder_texture_DCReconstruction_invpred_scheduler();
//extern void call_decoder_motion_interpolation_scheduler();
//extern void call_decoder_motion_add_scheduler();
//extern void call_decoder_motion_framebuf_scheduler();

void main(){
	 call_decoder_merger_scheduler();
	 call_decoder_parser_parseheaders_scheduler();
	 call_decoder_parser_mvseq_scheduler();
	 call_decoder_parser_blkexp_scheduler();
	 call_decoder_parser_mvrecon_scheduler();
	 call_decoder_texture_DCsplit_scheduler();
	 call_decoder_texture_IS_scheduler();
	 call_decoder_texture_IAP_scheduler();
	 call_decoder_texture_IQ_scheduler();
	 call_decoder_texture_idct2d_scheduler();
	 call_decoder_texture_DCReconstruction_addressing_scheduler();
	 call_decoder_texture_DCReconstruction_invpred_scheduler();
	 call_decoder_motion_interpolation_scheduler();
	 call_decoder_motion_add_scheduler();
	 call_decoder_motion_framebuf_scheduler();
}
