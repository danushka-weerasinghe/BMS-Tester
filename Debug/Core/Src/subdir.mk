################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CAN_BUS.c \
../Core/Src/Display.c \
../Core/Src/EEPROM.c \
../Core/Src/LED_BuiltIn.c \
../Core/Src/LTC6811.c \
../Core/Src/LTC681x.c \
../Core/Src/MOD_BUS.c \
../Core/Src/Menu.c \
../Core/Src/RTC.c \
../Core/Src/Switches.c \
../Core/Src/can.c \
../Core/Src/fatfs_sd.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/CAN_BUS.o \
./Core/Src/Display.o \
./Core/Src/EEPROM.o \
./Core/Src/LED_BuiltIn.o \
./Core/Src/LTC6811.o \
./Core/Src/LTC681x.o \
./Core/Src/MOD_BUS.o \
./Core/Src/Menu.o \
./Core/Src/RTC.o \
./Core/Src/Switches.o \
./Core/Src/can.o \
./Core/Src/fatfs_sd.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/CAN_BUS.d \
./Core/Src/Display.d \
./Core/Src/EEPROM.d \
./Core/Src/LED_BuiltIn.d \
./Core/Src/LTC6811.d \
./Core/Src/LTC681x.d \
./Core/Src/MOD_BUS.d \
./Core/Src/Menu.d \
./Core/Src/RTC.d \
./Core/Src/Switches.d \
./Core/Src/can.d \
./Core/Src/fatfs_sd.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Core/Inc/U8g2 Display" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Core/Inc/U8g2 Display" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Core/Inc/U8g2 Display" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/MIDWARE/FATFS_SD" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CAN_BUS.cyclo ./Core/Src/CAN_BUS.d ./Core/Src/CAN_BUS.o ./Core/Src/CAN_BUS.su ./Core/Src/Display.cyclo ./Core/Src/Display.d ./Core/Src/Display.o ./Core/Src/Display.su ./Core/Src/EEPROM.cyclo ./Core/Src/EEPROM.d ./Core/Src/EEPROM.o ./Core/Src/EEPROM.su ./Core/Src/LED_BuiltIn.cyclo ./Core/Src/LED_BuiltIn.d ./Core/Src/LED_BuiltIn.o ./Core/Src/LED_BuiltIn.su ./Core/Src/LTC6811.cyclo ./Core/Src/LTC6811.d ./Core/Src/LTC6811.o ./Core/Src/LTC6811.su ./Core/Src/LTC681x.cyclo ./Core/Src/LTC681x.d ./Core/Src/LTC681x.o ./Core/Src/LTC681x.su ./Core/Src/MOD_BUS.cyclo ./Core/Src/MOD_BUS.d ./Core/Src/MOD_BUS.o ./Core/Src/MOD_BUS.su ./Core/Src/Menu.cyclo ./Core/Src/Menu.d ./Core/Src/Menu.o ./Core/Src/Menu.su ./Core/Src/RTC.cyclo ./Core/Src/RTC.d ./Core/Src/RTC.o ./Core/Src/RTC.su ./Core/Src/Switches.cyclo ./Core/Src/Switches.d ./Core/Src/Switches.o ./Core/Src/Switches.su ./Core/Src/can.cyclo ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/can.su ./Core/Src/fatfs_sd.cyclo ./Core/Src/fatfs_sd.d ./Core/Src/fatfs_sd.o ./Core/Src/fatfs_sd.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

