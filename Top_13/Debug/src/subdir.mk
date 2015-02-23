################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_13/Top_13.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/Top_13.o 

C_DEPS += \
./src/Top_13.d 


# Each subdirectory must supply rules for building sources it contributes
src/Top_13.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_13/Top_13.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -w -O0 -g3 -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_13\src" -c -fmessage-length=0 -I../../bsp_cluster1_mb5/MbCluster1_mb5_microblaze_0/include -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mcpu=v9.3 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


