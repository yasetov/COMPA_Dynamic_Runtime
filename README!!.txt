Each folder in the depot corresponds to a Xilinx SDK project, except "RemoteSystemsTempFiles"
Description of the contents of each project :
- Actors
	- Source files of the actors, except Source and Display which are local to the microblaze 0.
	- File "schedCallers.c" contains the "jump functions" which are the entry points to the actors' code (see the paper summited to COMPASS'15 at https://scm.gforge.inria.fr/svn/compa/trunk/Papers/ComPAS2015)
- Top_xx 
	- For each processor, the "Main" function of the local runtime and eventually the source of resident actors (e.g. Source and Display in Top_00)
- Top_arm_00_
	- adv7511 contains the headers and the library to interface the ADV7511 Transmitter on the board
	- ffs is required by the file system library that is used to access the SD card
	- hdmi contains the sources for handling the HDMI controller on the FPGA
	- runtime contains the sources of the global runtime
- actors_bin
	- It is used to create the executable (.elf) with all the actors code which is loaded into memory at system initialization by the Cortex-A9.
- design_2x8MB_shbram_hdmi_psddr_ic_wrapper_hw_platform_0
	- Is the hardware platform 
- mpeg_libs_arm
	- Global files generated from Orcc, e.g. "fifoAllocations.h" and "fifo.h".
- runtime
	- Sources of the local runtime

The file "actorsBinScript.tcl" contains the script (dowActorsBin) that is executed at system initialization to load the actors binary into memory.