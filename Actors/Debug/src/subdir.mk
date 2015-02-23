################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_merger.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_add.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_framebuf.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_interpolation.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_blkexp.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_mvrecon.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_mvseq.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_parseheaders.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCReconstruction_addressing.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCReconstruction_invpred.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCsplit.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IAP.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IQ.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IS.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_idct2d.c \
C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/schedCallers.c 

OBJS += \
./src/decoder_merger.o \
./src/decoder_motion_add.o \
./src/decoder_motion_framebuf.o \
./src/decoder_motion_interpolation.o \
./src/decoder_parser_blkexp.o \
./src/decoder_parser_mvrecon.o \
./src/decoder_parser_mvseq.o \
./src/decoder_parser_parseheaders.o \
./src/decoder_texture_DCReconstruction_addressing.o \
./src/decoder_texture_DCReconstruction_invpred.o \
./src/decoder_texture_DCsplit.o \
./src/decoder_texture_IAP.o \
./src/decoder_texture_IQ.o \
./src/decoder_texture_IS.o \
./src/decoder_texture_idct2d.o \
./src/schedCallers.o 

C_DEPS += \
./src/decoder_merger.d \
./src/decoder_motion_add.d \
./src/decoder_motion_framebuf.d \
./src/decoder_motion_interpolation.d \
./src/decoder_parser_blkexp.d \
./src/decoder_parser_mvrecon.d \
./src/decoder_parser_mvseq.d \
./src/decoder_parser_parseheaders.d \
./src/decoder_texture_DCReconstruction_addressing.d \
./src/decoder_texture_DCReconstruction_invpred.d \
./src/decoder_texture_DCsplit.d \
./src/decoder_texture_IAP.d \
./src/decoder_texture_IQ.d \
./src/decoder_texture_IS.d \
./src/decoder_texture_idct2d.d \
./src/schedCallers.d 


# Each subdirectory must supply rules for building sources it contributes
src/decoder_merger.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_merger.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_motion_add.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_add.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_motion_framebuf.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_framebuf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_motion_interpolation.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_motion_interpolation.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_parser_blkexp.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_blkexp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_parser_mvrecon.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_mvrecon.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_parser_mvseq.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_mvseq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_parser_parseheaders.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_parser_parseheaders.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_DCReconstruction_addressing.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCReconstruction_addressing.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_DCReconstruction_invpred.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCReconstruction_invpred.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_DCsplit.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_DCsplit.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_IAP.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IAP.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_IQ.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IQ.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_IS.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_IS.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/decoder_texture_idct2d.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/decoder_texture_idct2d.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/schedCallers.o: C:/Users/yolivave/work/orcc-gen/mpeg4Visual/serial/Compa/src/Actors/schedCallers.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -O0 -g -I"C:\Users\yolivave\work\orcc-gen\mpeg4Visual\serial\Compa\libs\orcc\include" -c -fmessage-length=0 -mno-xl-reorder -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mno-xl-soft-div -mcpu=v9.3 -mno-xl-soft-mul -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


