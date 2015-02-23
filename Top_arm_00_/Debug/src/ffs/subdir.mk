################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ffs/ccsbcs.c 

OBJS += \
./src/ffs/ccsbcs.o 

C_DEPS += \
./src/ffs/ccsbcs.d 


# Each subdirectory must supply rules for building sources it contributes
src/ffs/%.o: ../src/ffs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM g++ compiler'
	arm-xilinx-eabi-g++ -O0 -g3 -I../src/adv7511/inc -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -I../../bsp_cortex0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


