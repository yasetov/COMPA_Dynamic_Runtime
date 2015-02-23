/*
 * mdm_dbg_reg.c
 *
 *  Created on: 22 janv. 2015
 *      Author: yolivave
 */




#define enableDBGAccess(BaseAddress) \
	Xil_Out32((BaseAddress+ 0x18), 0xebab)

#define set_DBG_CTRL(BaseAddress, value) \
	Xil_Out32((BaseAddress + 0x10), value)

#define get_DBG_STAT(BaseAddress) \
	Xil_In32(BaseAddress + 0x10)

#define set_DBG_DATA_32(BaseAddress, value) \
	Xil_Out32((BaseAddress + 0x14), value)

#define get_DBG_DATA_8(BaseAddress) \
	Xil_In8(BaseAddress + 0x14)

#define get_DBG_DATA_32(BaseAddress) \
	Xil_In8(BaseAddress + 0x14)

void startDebugRegAccessSeq(){
	u32 status;

	enableDBGAccess(XPAR_MDM_1_BASEADDR);		// Write magic number into DBG_LOCK

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x61A1F); // Will write which µb reg.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 1);	// Select µb 0.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A404); // Will write the PCCMDR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 1);	// Set the Reset bit to 1.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 0);	// Write event number for event counter 0

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 11);	// Write event number for event counter 1

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 12);	// Write event number for event counter 2

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 13);	// Write event number for event counter 3

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 30);	// Write event number for event counter 4

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A207); // Will write PCCTRLR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 63);	// Write event number for latency counter

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A404); // Will write the PCCMDR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 24);	// Set the Clear and Start bits to 1.
}

void perfMonitoring(){
	u8 cntrStatus;
	u32 status;
	u32 perfMonData;

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A404); // Will write the PCCMDR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 6);	// Set the Sample and Stop bits to 1.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A404); // Will write the PCCMDR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 1);	// Set the Reset bit to 1.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A601); 		// Will read the PCSR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	cntrStatus = get_DBG_DATA_8(XPAR_MDM_1_BASEADDR);	// Read status of event counter 0.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A601); 		// Will read the PCSR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	cntrStatus = get_DBG_DATA_8(XPAR_MDM_1_BASEADDR);	// Read status of event counter 1.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4A404); // Will write the PCCMDR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	set_DBG_DATA_32(XPAR_MDM_1_BASEADDR, 1);	// Set the Reset bit to 1.

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item for counter 0

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item for counter 1

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item for counter 2

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item for counter 3

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item for counter 4

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item 0 for latency counter

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item 1 for latency counter

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item 2 for latency counter

	set_DBG_CTRL(XPAR_MDM_1_BASEADDR, 0x4AC1F); 		// Will read the PCDRR.
	status = get_DBG_STAT(XPAR_MDM_1_BASEADDR);
	perfMonData = get_DBG_DATA_32(XPAR_MDM_1_BASEADDR); // Read data item 3 for latency counter

}
