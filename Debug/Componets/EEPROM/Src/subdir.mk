################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/EEPROM/Src/test.c 

OBJS += \
./Componets/EEPROM/Src/test.o 

C_DEPS += \
./Componets/EEPROM/Src/test.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/EEPROM/Src/%.o Componets/EEPROM/Src/%.su Componets/EEPROM/Src/%.cyclo: ../Componets/EEPROM/Src/%.c Componets/EEPROM/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/Button & LED/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/CAN/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/Digital Pot/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/Display/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/EEPROM/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/GPIO expander/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/INA229/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/ISO SPI/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/RS485/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/RTC/Inc" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/SD Card/Inc" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-EEPROM-2f-Src

clean-Componets-2f-EEPROM-2f-Src:
	-$(RM) ./Componets/EEPROM/Src/test.cyclo ./Componets/EEPROM/Src/test.d ./Componets/EEPROM/Src/test.o ./Componets/EEPROM/Src/test.su

.PHONY: clean-Componets-2f-EEPROM-2f-Src

