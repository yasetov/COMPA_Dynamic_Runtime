#include "xaxipmon_hw.h"
#include "xparameters.h"

u32 getMetricCounter(u32 BaseAddress, u32 CounterNum)
{
	u32 RegValue;

	if (CounterNum < 10 ) {
		RegValue = XAxiPmon_ReadReg(InstancePtr->Config.BaseAddress,
			(XAPM_SMC0_OFFSET + (CounterNum * 16)));
	}
	else if (CounterNum >= 10 && CounterNum < 12) {
		RegValue = XAxiPmon_ReadReg(InstancePtr->Config.BaseAddress,
			(XAPM_SMC10_OFFSET + ((CounterNum - 10) * 16)));
	}
	else if (CounterNum >= 12 && CounterNum < 24) {
		RegValue = XAxiPmon_ReadReg(InstancePtr->Config.BaseAddress,
			(XAPM_SMC12_OFFSET + ((CounterNum - 12) * 16)));
	}
	else if (CounterNum >= 24 && CounterNum < 36) {
		RegValue = XAxiPmon_ReadReg(InstancePtr->Config.BaseAddress,
			(XAPM_SMC24_OFFSET + ((CounterNum - 24) * 16)));
	}
	else
		RegValue = XAxiPmon_ReadReg(InstancePtr->Config.BaseAddress,
			(XAPM_SMC36_OFFSET + ((CounterNum - 36) * 16)));

	return RegValue;
}


void readPerfMonitor(u32 BaseAddress,
					u8 NbSlots,
					u32 Metrics[]){

	u8 SlotId = 0x0;

	/* Get Metric Counters  */
	for (SlotId = 0; SlotId < NbSlots; SlotId++) {
		Metrics[SlotId] = getMetricCounter(BaseAddress, SlotId);
//		Metrics[SlotId][1] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_1);
//		Metrics[SlotId][2] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_2);
//		Metrics[SlotId][3] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_3);

//		Incrementers[SlotId][0] = XAxiPmon_GetIncrementer(AxiPmonInstPtr, XAPM_INCREMENTER_0);
	}
}
