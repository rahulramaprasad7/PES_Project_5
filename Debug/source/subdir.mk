################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Project_5_UART.c \
../source/cbtestprev.c \
../source/circularBuffer.c \
../source/ledControl.c \
../source/loggerFunctions.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/uartIncludes.c 

OBJS += \
./source/Project_5_UART.o \
./source/cbtestprev.o \
./source/circularBuffer.o \
./source/ledControl.o \
./source/loggerFunctions.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/uartIncludes.o 

C_DEPS += \
./source/Project_5_UART.d \
./source/cbtestprev.d \
./source/circularBuffer.d \
./source/ledControl.d \
./source/loggerFunctions.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/uartIncludes.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -Ddebug -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


