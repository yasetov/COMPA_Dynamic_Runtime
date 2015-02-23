/*
 * perfMonMngr.c
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */
#include "stdio.h"
#include "config.h"
#include "perfMonMngr.h"


static XAxiPmon AxiPmonInst;     						/* System Monitor driver instance */

static XAxiPmon AxiPmonInst_00;    						/* Cluster_0 Monitor_0 driver instance */
static XAxiPmon AxiPmonInst_01;    						/* Cluster_0 Monitor_1 driver instance */
static XAxiPmon AxiPmonInst_02;    						/* Cluster_0 Monitor_2 driver instance */

static XAxiPmon AxiPmonInst_10;    						/* Cluster_1 Monitor_0 driver instance */
static XAxiPmon AxiPmonInst_11;    						/* Cluster_1 Monitor_1 driver instance */
static XAxiPmon AxiPmonInst_12;    						/* Cluster_1 Monitor_2 driver instance */


int init_apm(XAxiPmon *AxiPmonInstPtr, u16 DeviceID, u8 Metric)
{
	int Status;
	XAxiPmon_Config *ConfigPtr;
	u8 SlotId = 0x0;

	ConfigPtr = XAxiPmon_LookupConfig(DeviceID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	XAxiPmon_CfgInitialize(AxiPmonInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

//	XAxiPmon_EnableIDFilter(AxiPmonInstPtr);
	XAxiPmon_DisableIDFilter(AxiPmonInstPtr);

	/*
	 * Self Test the System Monitor/ADC device
	 */
	Status = XAxiPmon_SelfTest(AxiPmonInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Select Agent and required set of Metrics for a Counter.
	 * We can select another agent,Metrics for another counter by
	 * calling below function again.
	 */
	for (SlotId = 0; SlotId < ConfigPtr->NumberofSlots; SlotId++) {
		XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, Metric, SlotId);
//		XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, XAPM_METRIC_SET_1, XAPM_METRIC_COUNTER_1);
//		XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, XAPM_METRIC_SET_2, XAPM_METRIC_COUNTER_2);
//		XAxiPmon_SetMetrics(AxiPmonInstPtr, SlotId, XAPM_METRIC_SET_3, XAPM_METRIC_COUNTER_3);
	}


	/*
	 * Enable interrupts and if APM_1
	 */
//	if(DeviceID == XPAR_MBCLUSTER1_AXI_PERF_MON_0_DEVICE_ID){
//		XAxiPmon_IntrGlobalEnable(AxiPmonInstPtr);									// Enable Global Interrupt.
//		XAxiPmon_WriteReg(AxiPmonInstPtr->Config.BaseAddress, XAPM_IE_OFFSET, 2);	// Enable Sample Interval Counter Overflow Interrupt.
//		XAxiPmon_SetSampleInterval(AxiPmonInstPtr, 10000000); 						// Write the load value in the Sample Interval Register (1 s).
//		XAxiPmon_WriteReg(AxiPmonInstPtr->Config.BaseAddress, XAPM_SICR_OFFSET, 2);	// Set the Load bit in the Sample Interval Control Register.
//		XAxiPmon_EnableSampleIntervalCounter(AxiPmonInstPtr);						// Enable the Sample Interval Counter.
//	}

	/*
	 * Enable Metric Counters.
	 */
	XAxiPmon_EnableMetricsCounter(AxiPmonInstPtr);

	/*
	 * Enable Global Clock Counter Register.
	 */
	XAxiPmon_EnableGlobalClkCounter(AxiPmonInstPtr);

	return XST_SUCCESS;

}



int initPerfMons(){
	int Status;
	/* Initializing the AXI Performance Monitors*/

//	Status = init_apm(&AxiPmonInst, XPAR_AXI_PERF_MON_0_DEVICE_ID, XAPM_METRIC_SET_0);
//	if (Status != XST_SUCCESS) {
//		xil_printf("APM System initialization failed.\r\n");
//		return XST_FAILURE;
//	}
//	XAxiPmon_SetMetrics(&AxiPmonInst, 0, XAPM_METRIC_SET_1, 2);
//	XAxiPmon_SetMetrics(&AxiPmonInst, 1, XAPM_METRIC_SET_1, 3);


	Status = init_apm(&AxiPmonInst_00, XPAR_AXIPMON_0_DEVICE_ID, XAPM_METRIC_SET_1);
	if (Status != XST_SUCCESS) {
		xil_printf("APM_0 Cluster_1 initialization failed.\r\n");
		return XST_FAILURE;
	}
	Status = init_apm(&AxiPmonInst_10, XPAR_AXIPMON_2_DEVICE_ID, XAPM_METRIC_SET_1);
	if (Status != XST_SUCCESS) {
		xil_printf("APM_0 Cluster_2 initialization failed.\r\n");
		return XST_FAILURE;
	}
//	Status = init_apm(&AxiPmonInst_01, XPAR_MBCLUSTER1_AXI_PERF_MON_1_DEVICE_ID, XAPM_METRIC_SET_1);
//	if (Status != XST_SUCCESS) {
//		xil_printf("APM_1 Cluster_1 initialization failed.\r\n");
//		return XST_FAILURE;
//	}
//	Status = init_apm(&AxiPmonInst_11, XPAR_MBCLUSTER2_AXI_PERF_MON_1_DEVICE_ID, XAPM_METRIC_SET_1);
//	if (Status != XST_SUCCESS) {
//		xil_printf("APM_1 Cluster_2 initialization failed.\r\n");
//		return XST_FAILURE;
//	}

	Status = init_apm(&AxiPmonInst_02, XPAR_MBCLUSTER0_AXI_PERF_MON_2_DEVICE_ID, XAPM_METRIC_SET_2);
	if (Status != XST_SUCCESS) {
		xil_printf("APM_2 Cluster_0 initialization failed.\r\n");
		return XST_FAILURE;
	}
	Status = init_apm(&AxiPmonInst_12, XPAR_MBCLUSTER1_AXI_PERF_MON_2_DEVICE_ID, XAPM_METRIC_SET_2);
	if (Status != XST_SUCCESS) {
		xil_printf("APM_2 Cluster_1 initialization failed.\r\n");
		return XST_FAILURE;
	}
	/*****/
}


void readPerfMonitor(XAxiPmon *AxiPmonInstPtr,
					u32 Metrics[],
					u32 *ClkCntHigh,
					u32 *ClkCntLow){

	u8 SlotId = 0x0;
	/*
	 * Disable Global Clock Counter Register.
	 */

	XAxiPmon_DisableGlobalClkCounter(AxiPmonInstPtr);

	/*
	 * Disable Metric Counters.
	 */
//	XAxiPmon_DisableMetricsCounter(AxiPmonInstPtr);

	/* Get Metric Counters  */
	for (SlotId = 0; SlotId < AxiPmonInstPtr->Config.NumberofSlots; SlotId++) {
		Metrics[SlotId] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, SlotId);
//		Metrics[SlotId][1] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_1);
//		Metrics[SlotId][2] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_2);
//		Metrics[SlotId][3] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_3);

//		Incrementers[SlotId][0] = XAxiPmon_GetIncrementer(AxiPmonInstPtr, XAPM_INCREMENTER_0);
	}

	/* Get Global Clock Cycles Count in ClkCntHigh,ClkCntLow */
	XAxiPmon_GetGlobalClkCounter(AxiPmonInstPtr, ClkCntHigh, ClkCntLow);
}


void printWrittenBytesValues(u32 nbProcessors, u32 writtenBytes[NB_PROCESSORS][NB_ACTORS + 1]){
	u8 i, j;
	for (i = 0; i < nbProcessors; i++) {
		for (j = 0; j < NB_ACTORS; j++) {
			printf("proc %u actor %u : %u bytes\n\r", i, j, writtenBytes[i][j]);
		}
//		xil_printf("proc %ud total bytes : %ud bytes\n\r", i, writtenBytes[i][NB_ACTORS]);
		printf("proc %u total bytes : %u bytes\n\r", i, writtenBytes[i][NB_ACTORS]);
	}
}
