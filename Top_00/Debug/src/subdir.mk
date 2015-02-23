################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Top_00.c \
../src/display.c \
../src/display_MB.c \
../src/source.c \
../src/source_MB.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/Top_00.o \
./src/display.o \
./src/display_MB.o \
./src/source.o \
./src/source_MB.o 

C_DEPS += \
./src/Top_00.d \
./src/display.d \
./src/display_MB.d \
./src/source.d \
./src/source_MB.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\runtime\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Actors\src" -I"C:\Users\yolivave\work\Xilinx_SDK_workspace\Top_00\src" -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -fpic -I../../bsp_cluster0_mb0/MbCluster0_mb0_microblaze_0/include -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


