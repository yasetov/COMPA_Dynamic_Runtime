#include "string.h"
#include "xil_cache.h"
#include "xtmrctr.h"
#include "xaxipmon.h"
#include "ctrl_fifo_functions.h"
#include "config.h"
#include "actors.h"
#include "timerMngr.h"
#include "schedCallers.h"

///////////////////////////////////////////////////
//// Action schedulers
////extern int source_scheduler();
////extern int display_scheduler();
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

//////////////////////////////////////////////////////////
// Control FIFOs.
ctrl_fifo_hdlr ctrl_fifo_input;
ctrl_fifo_hdlr ctrl_fifo_output;
ctrl_fifo_hdlr input_data_fifo;


// Timer
XTmrCtr xtimer;
XTmrCtr* TmrCtrInstancePtr = &xtimer;

// APM
#define getNbWrittenBytes(AxiPmonInstPtr) \
	XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XPAR_CPU_ID%8)
XAxiPmon AxiPmon;

u8 fps = 0;	// Frame per seconds
u32 frameCtr = 0;

u8 init_done = 0;
u32 nbElements = 0;
u32 nbStaticActors = 2;
u32 nbDynActors = 0;
actorsNames sched_callers_array[NB_ACTORS] = {0};
u64 globalStartTime, globalStopTime;
u64 actorStartTime, actorStopTime;
u64 diffTime;
u64 execTimes[NB_ACTORS + 1] = {0}; // The last element stores the global execution time!
u32 actorWrittenByteStart, actorWrittenByteStop;
u32 writtenBytes[NB_ACTORS + 1] = {0}; // The last element stores the total number of written bytes!


void init_timer(){
	int Status;

	// Initialize device and link it to a handler. It is initialized by the ARM.
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, XPAR_TIMERS_AXI_TIMER_0_DEVICE_ID);
	if ((Status != XST_SUCCESS) || (Status != XST_DEVICE_IS_STARTED)) {
//		TODO: ..handle error
	}

	// Reset timer
//	XTmrCtr_SetResetValue(TmrCtrInstancePtr, TIMER_CNTR_0, stopTime);
//	XTmrCtr_SetOptions(TmrCtrInstancePtr, TIMER_CNTR_0,
//						XTC_AUTO_RELOAD_OPTION |
//						XTC_CASCADE_MODE_OPTION);
//	XTmrCtr_Reset(TmrCtrInstancePtr, TIMER_CNTR_0);
}


int init_apm(XAxiPmon *AxiPmonInstPtr, u16 DeviceID, u8 Metric)
{
	int Status;
	XAxiPmon_Config *ConfigPtr;
	u8 SlotId = 0x0;

	ConfigPtr = XAxiPmon_LookupConfig(DeviceID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	AxiPmonInstPtr->IsReady = XIL_COMPONENT_IS_READY;
	AxiPmonInstPtr->Mode = XAPM_MODE_ADVANCED;
	AxiPmonInstPtr->Config.DeviceId = ConfigPtr->DeviceId;
	AxiPmonInstPtr->Config.BaseAddress = ConfigPtr->BaseAddress;

	return XST_SUCCESS;

}

void main(){
	u32 i;
	actorsNames schedCaller;

	init_timer();
	init_apm(&AxiPmon, XPAR_AXIPMON_0_DEVICE_ID, XAPM_METRIC_SET_2);

//	startDebugRegAccessSeq();

	// Initializing control FIFOs.
	init_ctrl_fifo_hdlr(&ctrl_fifo_input, CTRL_FIFO_IN_0_START_ADDR, CTRL_FIFO_IN_0_RD_IX_ADDR, CTRL_FIFO_IN_0_WR_IX_ADDR, CTRL_FIFO_IN_0_SIZE);
	init_ctrl_fifo_hdlr(&ctrl_fifo_output, CTRL_FIFO_OUT_0_START_ADDR, CTRL_FIFO_OUT_0_RD_IX_ADDR, CTRL_FIFO_OUT_0_WR_IX_ADDR, CTRL_FIFO_OUT_0_SIZE);

	// Initializing FIFO with input data.
	init_ctrl_fifo_hdlr(&input_data_fifo, INPUT_FILE_START_ADDR, INPUT_FILE_RD_IX_ADDR, INPUT_FILE_WR_IX_ADDR, INPUT_FILE_SIZE);

//	microblaze_enable_dcache();
	microblaze_enable_icache();
	while(1){
		// Reading input control FIFO to get the actor(s) to be executed.
		u8 msg = rcvCtrlMsgType(&ctrl_fifo_input);
		switch (msg) {
			case MSG_INIT_DONE:
				init_done = 1;
				break;
			case MSG_ACTORS_MAP:
				nbElements = pop_contents_input_ctrl_fifo_blocking(&ctrl_fifo_input, (u8*)sched_callers_array);
				nbDynActors = nbElements/sizeof(int); // FIFO elements are bytes while scheduler addresses are words!
				break;
			case MSG_FLUSH_DCACHE:
				microblaze_flush_dcache();
				mbar(0);
				sendCtrlMsgType_blocking(&ctrl_fifo_output, MSG_FLUSH_DCACHE_DONE);
				nbElements = 0;
				nbDynActors = 0;
				memset(execTimes, 0, sizeof(execTimes));
				memset(writtenBytes, 0, sizeof(writtenBytes));
//				XAxiPmon_ResetMetricCounter(&AxiPmon);
				break;
			case MSG_GET_METRICS:
//				perfMonitoring();
				sendCtrlMsgType_blocking(&ctrl_fifo_output, MSG_GET_METRICS_OK);
				push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output, (u8*)execTimes, sizeof(execTimes));
				push_contents_output_ctrl_fifo_blocking(&ctrl_fifo_output, (u8*)writtenBytes, sizeof(writtenBytes));
				break;
			default:
				break;
		}

		if((init_done) && ((nbStaticActors > 0) || (nbDynActors > 0))){
//			globalStartTime = XTmrCtr_GetValue(TmrCtrInstancePtr, TIMER_CNTR_0);
			globalStartTime = getTimerValue64();

			// Calling scheduler of static actors
			source_scheduler();
			display_scheduler();

			// Calling the scheduler of each actor (if any).
			for (i = 0; i < nbDynActors; i++) {
//				actorStartTime = XTmrCtr_GetValue(TmrCtrInstancePtr, TIMER_CNTR_0);
				actorStartTime = getTimerValue64();
				actorWrittenByteStart = getNbWrittenBytes(&AxiPmon);

				schedCaller = sched_callers_array[i];
				switch (schedCaller) {
					case decoder_merger:
						call_decoder_merger_scheduler();
						break;
					case decoder_parser_parseheaders:
						call_decoder_parser_parseheaders_scheduler();
						break;
					case decoder_parser_mvseq:
						call_decoder_parser_mvseq_scheduler();
						break;
					case decoder_parser_blkexp:
						call_decoder_parser_blkexp_scheduler();
						break;
					case decoder_parser_mvrecon:
						call_decoder_parser_mvrecon_scheduler();
						break;
					case decoder_texture_DCsplit:
						call_decoder_texture_DCsplit_scheduler();
						break;
					case decoder_texture_IS:
						call_decoder_texture_IS_scheduler();
						break;
					case decoder_texture_IAP:
						call_decoder_texture_IAP_scheduler();
						break;
					case decoder_texture_IQ:
						call_decoder_texture_IQ_scheduler();
						break;
					case decoder_texture_idct2d:
						call_decoder_texture_idct2d_scheduler();
						break;
					case decoder_texture_DCReconstruction_addressing:
						call_decoder_texture_DCReconstruction_addressing_scheduler();
						break;
					case decoder_texture_DCReconstruction_invpred:
						call_decoder_texture_DCReconstruction_invpred_scheduler();
						break;
					case decoder_motion_interpolation:
						call_decoder_motion_interpolation_scheduler();
						break;
					case decoder_motion_add:
						call_decoder_motion_add_scheduler();
						break;
					case decoder_motion_framebuf:
						call_decoder_motion_framebuf_scheduler();
						break;
					default:
						break;
				}
				actorStopTime = getTimerValue64();
				diffTime = actorStopTime - actorStartTime;
				execTimes[schedCaller] += (diffTime);

				actorWrittenByteStop = getNbWrittenBytes(&AxiPmon);
				writtenBytes[schedCaller] += (actorWrittenByteStop - actorWrittenByteStart);
			}
			globalStopTime = getTimerValue64();
			diffTime = globalStopTime - globalStartTime;
			execTimes[NB_ACTORS] += (diffTime);

			writtenBytes[NB_ACTORS] += getNbWrittenBytes(&AxiPmon);
		}
	}
}
