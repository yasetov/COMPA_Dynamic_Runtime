################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ctrlComms.c \
C:/Users/yolivave/work/Xilinx_SDK_workspace/runtime/src/ctrl_fifo_functions.c \
../src/ctrl_fifos.c \
../src/fileMgr.c \
../src/main.c \
../src/mappings.c \
../src/perfMonMngr.c \
../src/printDataFifoIx.c \
../src/system.c \
../src/timerMngr.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/ctrlComms.o \
./src/ctrl_fifo_functions.o \
./src/ctrl_fifos.o \
./src/fileMgr.o \
./src/main.o \
./src/mappings.o \
./src/perfMonMngr.o \
./src/printDataFifoIx.o \
./src/system.o \
./src/timerMngr.o 

C_DEPS += \
./src/ctrlComms.d \
./src/ctrl_fifo_functions.d \
./src/ctrl_fifos.d \
./src/fileMgr.d \
./src/main.d \
./src/mappings.d \
./src/perfMonMngr.d \
./src/printDataFifoIx.d \
./src/system.d \
./src/timerMngr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -O0 -g3 -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_arm_00\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\Downloads\no-OS-master\adv7511\library\zc\inc" -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -I../../bsp_cortex0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ctrl_fifo_functions.o: C:/Users/yolivave/work/Xilinx_SDK_workspace/runtime/src/ctrl_fifo_functions.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -O0 -g3 -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_arm_00\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\Downloads\no-OS-master\adv7511\library\zc\inc" -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -I../../bsp_cortex0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


