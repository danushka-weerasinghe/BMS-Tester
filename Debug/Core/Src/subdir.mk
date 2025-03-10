################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/INA229.c \
../Core/Src/PCAL9535A.c \
../Core/Src/Temp_controller.c \
../Core/Src/calibration.c \
../Core/Src/config.c \
../Core/Src/display.c \
../Core/Src/gpio_control.c \
../Core/Src/gpio_expander.c \
../Core/Src/i2c_scanner.c \
../Core/Src/main.c \
../Core/Src/mcu.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/INA229.o \
./Core/Src/PCAL9535A.o \
./Core/Src/Temp_controller.o \
./Core/Src/calibration.o \
./Core/Src/config.o \
./Core/Src/display.o \
./Core/Src/gpio_control.o \
./Core/Src/gpio_expander.o \
./Core/Src/i2c_scanner.o \
./Core/Src/main.o \
./Core/Src/mcu.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/INA229.d \
./Core/Src/PCAL9535A.d \
./Core/Src/Temp_controller.d \
./Core/Src/calibration.d \
./Core/Src/config.d \
./Core/Src/display.d \
./Core/Src/gpio_control.d \
./Core/Src/gpio_expander.d \
./Core/Src/i2c_scanner.d \
./Core/Src/main.d \
./Core/Src/mcu.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Core/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Core/Inc/u8g2" -I"D:/GITHUB/BMS-Tester/Core/Inc/u8g2" -I"D:/GITHUB/BMS-Tester/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/INA229.cyclo ./Core/Src/INA229.d ./Core/Src/INA229.o ./Core/Src/INA229.su ./Core/Src/PCAL9535A.cyclo ./Core/Src/PCAL9535A.d ./Core/Src/PCAL9535A.o ./Core/Src/PCAL9535A.su ./Core/Src/Temp_controller.cyclo ./Core/Src/Temp_controller.d ./Core/Src/Temp_controller.o ./Core/Src/Temp_controller.su ./Core/Src/calibration.cyclo ./Core/Src/calibration.d ./Core/Src/calibration.o ./Core/Src/calibration.su ./Core/Src/config.cyclo ./Core/Src/config.d ./Core/Src/config.o ./Core/Src/config.su ./Core/Src/display.cyclo ./Core/Src/display.d ./Core/Src/display.o ./Core/Src/display.su ./Core/Src/gpio_control.cyclo ./Core/Src/gpio_control.d ./Core/Src/gpio_control.o ./Core/Src/gpio_control.su ./Core/Src/gpio_expander.cyclo ./Core/Src/gpio_expander.d ./Core/Src/gpio_expander.o ./Core/Src/gpio_expander.su ./Core/Src/i2c_scanner.cyclo ./Core/Src/i2c_scanner.d ./Core/Src/i2c_scanner.o ./Core/Src/i2c_scanner.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mcu.cyclo ./Core/Src/mcu.d ./Core/Src/mcu.o ./Core/Src/mcu.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

