################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/gpio_driver.c \
../Drivers/Src/i2c_driver.c \
../Drivers/Src/rcc_driver.c \
../Drivers/Src/spi_driver.c \
../Drivers/Src/uart_driver.c 

OBJS += \
./Drivers/Src/gpio_driver.o \
./Drivers/Src/i2c_driver.o \
./Drivers/Src/rcc_driver.o \
./Drivers/Src/spi_driver.o \
./Drivers/Src/uart_driver.o 

C_DEPS += \
./Drivers/Src/gpio_driver.d \
./Drivers/Src/i2c_driver.d \
./Drivers/Src/rcc_driver.d \
./Drivers/Src/spi_driver.d \
./Drivers/Src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"D:/Important/Embedded-C/MCU1/004Driver/Drivers/Inc" -I"D:/Important/Embedded-C/MCU1/004Driver/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/gpio_driver.d ./Drivers/Src/gpio_driver.o ./Drivers/Src/gpio_driver.su ./Drivers/Src/i2c_driver.d ./Drivers/Src/i2c_driver.o ./Drivers/Src/i2c_driver.su ./Drivers/Src/rcc_driver.d ./Drivers/Src/rcc_driver.o ./Drivers/Src/rcc_driver.su ./Drivers/Src/spi_driver.d ./Drivers/Src/spi_driver.o ./Drivers/Src/spi_driver.su ./Drivers/Src/uart_driver.d ./Drivers/Src/uart_driver.o ./Drivers/Src/uart_driver.su

.PHONY: clean-Drivers-2f-Src

