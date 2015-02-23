cd C:/Users/yolivave/work/Xilinx_SDK_workspace
#cd C:/Users/yolivave/work/Xilinx_SDK_workspace/actors_bin/Debug

#source C:/Users/yolivave/work/Xilinx_SDK_workspace/debug_actorsBin_script.tcl
source C:/Users/yolivave/work/Xilinx_SDK_workspace/design_2x8MB_shbram_hdmi_psddr_ic_wrapper_hw_platform_0/ps7_init.tcl

proc printState {name value} {
	puts "$name - $value"
}

proc printActorStates {} {
	puts "Actor - State"
	printState merger [mrd_phys 0x20049d90]
	printState motion_add [mrd_phys 0x20049db4]
	printState motion_framebuffer [mrd_phys 0x200629fc]
	printState parser_blkexp [mrd_phys 0x20062abc]
	printState parser_mvrecon [mrd_phys 0x20062b10]
	printState parser_mvseq [mrd_phys 0x20062b28]
	printState parser_parseheaders [mrd_phys 0x20062be4]
	printState texture_DCReconstruction_addressing [mrd_phys 0x20062c10]
	printState texture_DCReconstruction_invpred [mrd_phys 0x20062c80]
	printState texture_IAP [mrd_phys 0x20063acc]
	printState testure_IQ [mrd_phys 0x20063af4]
}

proc printActorsInit {} {
	puts [mrd_phys 0x20046fac 1 b]
	puts [mrd_phys 0x2004951c 1 b]
	puts [mrd_phys 0x20049174 1 b]
	puts [mrd_phys 0x20048588 1 b]
	puts [mrd_phys 0x200477f8 1 b]
	puts [mrd_phys 0x20046820 1 b]
	puts [mrd_phys 0x20047fcc 1 b]
	puts [mrd_phys 0x200498c0 1 b]
	puts [mrd_phys 0x20046478 1 b]
	puts [mrd_phys 0x20046c00 1 b]
	puts [mrd_phys 0x20049c5c 1 b]
	puts [mrd_phys 0x20047348 1 b]
	puts [mrd_phys 0x20047c1e 1 b]
	puts [mrd_phys 0x20048dd8 1 b]
	puts [mrd_phys 0x20048a20 1 b]
}

proc dowActorsBin {} {
	connect arm hw
	ps7_init
	ps7_post_config
	dow C:/Users/yolivave/work/Xilinx_SDK_workspace/actors_bin/Debug/actors_bin.elf
	disconnect 64
}

dowActorsBin