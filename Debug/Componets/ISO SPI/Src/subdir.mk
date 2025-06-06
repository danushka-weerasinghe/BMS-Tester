################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/ISO\ SPI/Src/LTC6811.c \
../Componets/ISO\ SPI/Src/LTC681x.c \
../Componets/ISO\ SPI/Src/LTC68xx_API.c 

OBJS += \
./Componets/ISO\ SPI/Src/LTC6811.o \
./Componets/ISO\ SPI/Src/LTC681x.o \
./Componets/ISO\ SPI/Src/LTC68xx_API.o 

C_DEPS += \
./Componets/ISO\ SPI/Src/LTC6811.d \
./Componets/ISO\ SPI/Src/LTC681x.d \
./Componets/ISO\ SPI/Src/LTC68xx_API.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/ISO\ SPI/Src/LTC6811.o: ../Componets/ISO\ SPI/Src/LTC6811.c Componets/ISO\ SPI/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/GITHUB/BMS-Tester/Componets/Button & LED/Inc" -I"D:/GITHUB/BMS-Tester/Componets/CAN/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Digital Pot/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Display/Inc" -I"D:/GITHUB/BMS-Tester/Componets/EEPROM/Inc" -I"D:/GITHUB/BMS-Tester/Componets/GPIO expander/Inc" -I"D:/GITHUB/BMS-Tester/Componets/INA229/Inc" -I"D:/GITHUB/BMS-Tester/Componets/ISO SPI/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RS485/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RTC/Inc" -I"D:/GITHUB/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Componets/ISO SPI/Src/LTC6811.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Componets/ISO\ SPI/Src/LTC681x.o: ../Componets/ISO\ SPI/Src/LTC681x.c Componets/ISO\ SPI/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/GITHUB/BMS-Tester/Componets/Button & LED/Inc" -I"D:/GITHUB/BMS-Tester/Componets/CAN/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Digital Pot/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Display/Inc" -I"D:/GITHUB/BMS-Tester/Componets/EEPROM/Inc" -I"D:/GITHUB/BMS-Tester/Componets/GPIO expander/Inc" -I"D:/GITHUB/BMS-Tester/Componets/INA229/Inc" -I"D:/GITHUB/BMS-Tester/Componets/ISO SPI/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RS485/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RTC/Inc" -I"D:/GITHUB/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Componets/ISO SPI/Src/LTC681x.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Componets/ISO\ SPI/Src/LTC68xx_API.o: ../Componets/ISO\ SPI/Src/LTC68xx_API.c Componets/ISO\ SPI/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/GITHUB/BMS-Tester/Componets/Button & LED/Inc" -I"D:/GITHUB/BMS-Tester/Componets/CAN/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Digital Pot/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Display/Inc" -I"D:/GITHUB/BMS-Tester/Componets/EEPROM/Inc" -I"D:/GITHUB/BMS-Tester/Componets/GPIO expander/Inc" -I"D:/GITHUB/BMS-Tester/Componets/INA229/Inc" -I"D:/GITHUB/BMS-Tester/Componets/ISO SPI/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RS485/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RTC/Inc" -I"D:/GITHUB/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Componets/ISO SPI/Src/LTC68xx_API.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-ISO-20-SPI-2f-Src

clean-Componets-2f-ISO-20-SPI-2f-Src:
	-$(RM) ./Componets/ISO\ SPI/Src/LTC6811.cyclo ./Componets/ISO\ SPI/Src/LTC6811.d ./Componets/ISO\ SPI/Src/LTC6811.o ./Componets/ISO\ SPI/Src/LTC6811.su ./Componets/ISO\ SPI/Src/LTC681x.cyclo ./Componets/ISO\ SPI/Src/LTC681x.d ./Componets/ISO\ SPI/Src/LTC681x.o ./Componets/ISO\ SPI/Src/LTC681x.su ./Componets/ISO\ SPI/Src/LTC68xx_API.cyclo ./Componets/ISO\ SPI/Src/LTC68xx_API.d ./Componets/ISO\ SPI/Src/LTC68xx_API.o ./Componets/ISO\ SPI/Src/LTC68xx_API.su

.PHONY: clean-Componets-2f-ISO-20-SPI-2f-Src

