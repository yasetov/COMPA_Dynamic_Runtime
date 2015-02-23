################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_04/Top_04.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/Top_04.o 

C_DEPS += \
./src/Top_04.d 


# Each subdirectory must supply rules for building sources it contributes
src/Top_04.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Top_04/Top_04.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -w -O0 -g3 -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_04\src" -c -fmessage-length=0 -I../../bsp_cluster0_mb4/MbCluster0_mb4_microblaze_0/include -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mcpu=v9.3 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


