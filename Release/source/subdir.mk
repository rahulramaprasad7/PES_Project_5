################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Project_5_UART.c \
../source/System.c \
../source/circularBuffer.c \
../source/ledControl.c \
../source/loggerFunctions.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/uartIncludes.c \
../source/unitTest.c 

OBJS += \
./source/Project_5_UART.o \
./source/System.o \
./source/circularBuffer.o \
./source/ledControl.o \
./source/loggerFunctions.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/uartIncludes.o \
./source/unitTest.o 

C_DEPS += \
./source/Project_5_UART.d \
./source/System.d \
./source/circularBuffer.d \
./source/ledControl.d \
./source/loggerFunctions.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/uartIncludes.d \
./source/unitTest.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -Drelease -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


