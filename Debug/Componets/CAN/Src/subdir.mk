################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/CAN/Src/test.c 

OBJS += \
./Componets/CAN/Src/test.o 

C_DEPS += \
./Componets/CAN/Src/test.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/CAN/Src/%.o Componets/CAN/Src/%.su Componets/CAN/Src/%.cyclo: ../Componets/CAN/Src/%.c Componets/CAN/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/GITHUB/BMS-Tester/Componets/Button & LED/Inc" -I"D:/GITHUB/BMS-Tester/Componets/CAN/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Digital Pot/Inc" -I"D:/GITHUB/BMS-Tester/Componets/Display/Inc" -I"D:/GITHUB/BMS-Tester/Componets/EEPROM/Inc" -I"D:/GITHUB/BMS-Tester/Componets/GPIO expander/Inc" -I"D:/GITHUB/BMS-Tester/Componets/INA229/Inc" -I"D:/GITHUB/BMS-Tester/Componets/ISO SPI/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RS485/Inc" -I"D:/GITHUB/BMS-Tester/Componets/RTC/Inc" -I"D:/GITHUB/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-CAN-2f-Src

clean-Componets-2f-CAN-2f-Src:
	-$(RM) ./Componets/CAN/Src/test.cyclo ./Componets/CAN/Src/test.d ./Componets/CAN/Src/test.o ./Componets/CAN/Src/test.su

.PHONY: clean-Componets-2f-CAN-2f-Src

