################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/SD\ Card/Src/test.c 

OBJS += \
./Componets/SD\ Card/Src/test.o 

C_DEPS += \
./Componets/SD\ Card/Src/test.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/SD\ Card/Src/test.o: ../Componets/SD\ Card/Src/test.c Componets/SD\ Card/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/Button & LED/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/CAN/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/Digital Pot/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/Display/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/EEPROM/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/GPIO expander/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/INA229/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/ISO SPI/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/RS485/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/RTC/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/SD Card/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Componets/SD Card/Src/test.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-SD-20-Card-2f-Src

clean-Componets-2f-SD-20-Card-2f-Src:
	-$(RM) ./Componets/SD\ Card/Src/test.cyclo ./Componets/SD\ Card/Src/test.d ./Componets/SD\ Card/Src/test.o ./Componets/SD\ Card/Src/test.su

.PHONY: clean-Componets-2f-SD-20-Card-2f-Src

