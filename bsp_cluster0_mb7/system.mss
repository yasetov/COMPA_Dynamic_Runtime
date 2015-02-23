
 PARAMETER NAME = C:\Users\yolivave\work\Xilinx_SDK_workspace\bsp_cluster0_mb7\system.mss

 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = standalone
 PARAMETER OS_VER = 4.1
 PARAMETER PROC_INSTANCE = MbCluster0_mb7_microblaze_0
 PARAMETER stdin = mdm_1
 PARAMETER stdout = mdm_1
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu
 PARAMETER DRIVER_VER = 2.1
 PARAMETER HW_INSTANCE = MbCluster0_mb7_microblaze_0
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_ddr_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.0
 PARAMETER HW_INSTANCE = MbCluster0_mb7_microblaze_0_local_memory_lmb_bram_cntlr_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.0
 PARAMETER HW_INSTANCE = MbCluster0_mb7_microblaze_0_local_memory_lmb_bram_cntlr_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.0
 PARAMETER HW_INSTANCE = MbCluster0_mb7_microblaze_0_local_memory_lmb_bram_cntlr_2
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.0
 PARAMETER HW_INSTANCE = SharedBRAM_128Kb_axi_bram_ctrl_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 3.0
 PARAMETER HW_INSTANCE = mdm_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = tmrctr
 PARAMETER DRIVER_VER = 3.0
 PARAMETER HW_INSTANCE = timers_axi_timer_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = axipmon
 PARAMETER DRIVER_VER = 6.1
 PARAMETER HW_INSTANCE = MbCluster0_axi_perf_mon_2
END


