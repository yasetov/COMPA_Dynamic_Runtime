#cd C:/Users/yolivave/work/Xilinx_SDK_workspace
#cd C:/Users/yolivave/work/Xilinx_SDK_workspace/actors_bin/Debug
#source ./hw_platform_compa/ps7_init.tcl
#source ./design_2x8MB_shbram_hdmi_psddr_ic_wrapper_hw_platform_2/ps7_init.tcl
#connect arm hw
#ps7_init
#ps7_post_config
#fpga -f ./design_2x8MB_shbram_hdmi_psddr_ic_wrapper_hw_platform_2/design_2x8MB_shbram_hdmi_psddr_ic_wrapper.bit

proc connectAll {} {
	for {set i 1} {$i < 2} {incr i} {
		#apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Slave "/processing_system7_0/S_AXI_HP1" }  [get_bd_intf_pins mb$i\/M_AXI_IC]
		#set_property range 128K [get_bd_addr_segs {processing_system7_0/Data/SEG_axi_bram_ctrl_1_Mem0}]
		#set_property range 128K [get_bd_addr_segs mb$i/microblaze_0/Data/SEG_axi_bram_ctrl_0_Mem0]
		connect mb mdm -debugdevice cpunr $i
	}
}

proc connectY {} {
	for {set i 1} {$i < 16} {incr i} {
		if {$i == 3 || $i == 8} {
		} else {
			connect mb mdm -debugdevice cpunr $i
		}
	}
}

proc connectARM {} {
	connect arm hw
}

proc ps7 {} {
	ps7_init
	ps7_post_config
}

proc continueAll {} {
	for {set i 0} {$i < 15} {incr i} {
		targets $i
		con
	}
}

proc continueY {} {
	for {set i 0} {$i < 15} {incr i} {
		if {$i == 2 || $i == 7} {
		} else {
			targets $i
			con
		}
	}
}

proc stopAll {} {
	for {set i 0} {$i < 16} {incr i} {
		targets $i
		stop
	}
}

proc stopY {} {
	for {set i 0} {$i < 15} {incr i} {
		if {$i == 2 || $i == 7} {
		} else {
			targets $i
			stop
		}
	}
}

proc resetAll {} {
	for {set i 0} {$i < 15} {incr i} {
		targets $i
		rst
	}
}

proc resetY {} {
	for {set i 0} {$i < 15} {incr i} {
		if {$i == 2 || $i == 7} {
		} else {
			targets $i
			rst
		}
	}
}

proc disconnectAll {} {
	for {set i 0} {$i < 15} {incr i} {
		disconnect $i
	}
	targets 64
	disconnect 64
}

proc disconnectY {} {
	for {set i 0} {$i < 15} {incr i} {
		if {$i == 2 || $i == 7} {
		} else {
			disconnect $i
		}
	}
	targets 64
	disconnect 64
}

proc dowX {} {
	targets 2
	dow decoder_merger/Debug/decoder_merger.elf
	targets 7
	dow ../decoder_parser_parseheaders/Debug/decoder_parser_parseheaders.elf
}

proc dow7 {} {
	targets 7
	dow ../decoder_parser_parseheaders/Debug/decoder_parser_parseheaders.elf
}

proc dowARM {} {
	targets 64
	dow ../Src_Dsply/Debug/Src_Dsply.elf
}

proc initMem {} {
	targets 64
	dow ../arm_init/Debug/arm_init.elf
	run
}

proc checkFifos {} {
	puts -nonewline "fifo_0 "
	mrd 0x40000204
	mrd 0x40000204
}

proc disAll {} {
	for {set i 0} {$i < 15} {incr i} {
		targets $i
		dis
	}	
}

proc test {} {
	fpga -f ./myHDMItx_hw_platform/system.bit
}

proc test_debug {} {
	fpga -f ./hw_platform_1/download_debug.bit
	#connectARM
	#ps7_init
	#ps7_post_config
	#connectAll
	#resetAll
}

proc test_release {} {
	fpga -f ./hw_platform_1/download_release.bit
	#connectARM
	#ps7_init
	#ps7_post_config
	#connectAll
	#resetAll
}

proc end {} {
	rst -srst
	#xdisconnect -cable
	#disconnect 0
}

proc restart {} {
	end
	test_debug
}

proc testDDR {} {
	for {set i 0} {$i<8} {incr i} {
		targets $i
		mrd 0x20000000
	}
}

proc bootLinux {} {
	connect arm hw -debugdevice cpunr 1
	dow fsbl.elf
	con
	exec sleep 3
	stop
	dow -data devictree.dtb <devicetree blob address>
	dow -data uramdisk.image.gz <uramdisk address>
	dow -data uImage <uImage address>
	dow u-boot.elf
	con
}

proc dow_actors_bin {} {
	connect arm hw
	source ./design_2x8MB_shbram_hdmi_psddr_ic_wrapper_hw_platform_2/ps7_init.tcl
	ps7_init
	ps7_post_config
	dow ./actors_bin/Debug/actors_bin.elf
	disconnect 64
}

