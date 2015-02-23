################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_01/Top_01.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/Top_01.o 

C_DEPS += \
./src/Top_01.d 


# Each subdirectory must supply rules for building sources it contributes
src/Top_01.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_01/Top_01.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_01\src" -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -I../../bsp_cluster0_mb1/MbCluster0_mb1_microblaze_0/include -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


