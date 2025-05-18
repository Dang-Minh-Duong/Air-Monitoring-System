################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc_init.c \
../Core/Src/adc_read.c \
../Core/Src/gpio_init.c \
../Core/Src/main.c \
../Core/Src/motor_control.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_clock.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim2_pwm.c \
../Core/Src/usart1.c 

OBJS += \
./Core/Src/adc_init.o \
./Core/Src/adc_read.o \
./Core/Src/gpio_init.o \
./Core/Src/main.o \
./Core/Src/motor_control.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_clock.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim2_pwm.o \
./Core/Src/usart1.o 

C_DEPS += \
./Core/Src/adc_init.d \
./Core/Src/adc_read.d \
./Core/Src/gpio_init.d \
./Core/Src/main.d \
./Core/Src/motor_control.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_clock.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim2_pwm.d \
./Core/Src/usart1.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc_init.cyclo ./Core/Src/adc_init.d ./Core/Src/adc_init.o ./Core/Src/adc_init.su ./Core/Src/adc_read.cyclo ./Core/Src/adc_read.d ./Core/Src/adc_read.o ./Core/Src/adc_read.su ./Core/Src/gpio_init.cyclo ./Core/Src/gpio_init.d ./Core/Src/gpio_init.o ./Core/Src/gpio_init.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor_control.cyclo ./Core/Src/motor_control.d ./Core/Src/motor_control.o ./Core/Src/motor_control.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_clock.cyclo ./Core/Src/system_clock.d ./Core/Src/system_clock.o ./Core/Src/system_clock.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim2_pwm.cyclo ./Core/Src/tim2_pwm.d ./Core/Src/tim2_pwm.o ./Core/Src/tim2_pwm.su ./Core/Src/usart1.cyclo ./Core/Src/usart1.d ./Core/Src/usart1.o ./Core/Src/usart1.su

.PHONY: clean-Core-2f-Src

