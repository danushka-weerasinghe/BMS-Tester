################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/RS485/Src/modbusSlave.c \
../Componets/RS485/Src/modbus_crc.c 

OBJS += \
./Componets/RS485/Src/modbusSlave.o \
./Componets/RS485/Src/modbus_crc.o 

C_DEPS += \
./Componets/RS485/Src/modbusSlave.d \
./Componets/RS485/Src/modbus_crc.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/RS485/Src/%.o Componets/RS485/Src/%.su Componets/RS485/Src/%.cyclo: ../Componets/RS485/Src/%.c Componets/RS485/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/GITHUB/BMS-Tester/Componets/Button & LED/Inc" -I"D:/GITHUB/BMS-Tester/Componets/CAN/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Digital Pot/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Display/Inc" -I"D:/GITHUB/BMS-Tester/Componets/EEPROM/Inc" -I"D:/GITHUB/BMS-Tester/Componets/GPIO expander/Inc" -I"D:/GITHUB/BMS-Tester/Componets/INA229/Inc" -I"D:/GITHUB/BMS-Tester/Componets/ISO SPI/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RS485/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RTC/Inc" -I"D:/GITHUB/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-RS485-2f-Src

clean-Componets-2f-RS485-2f-Src:
	-$(RM) ./Componets/RS485/Src/modbusSlave.cyclo ./Componets/RS485/Src/modbusSlave.d ./Componets/RS485/Src/modbusSlave.o ./Componets/RS485/Src/modbusSlave.su ./Componets/RS485/Src/modbus_crc.cyclo ./Componets/RS485/Src/modbus_crc.d ./Componets/RS485/Src/modbus_crc.o ./Componets/RS485/Src/modbus_crc.su

.PHONY: clean-Componets-2f-RS485-2f-Src

