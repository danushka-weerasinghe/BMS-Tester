################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/GPIO\ expander/Src/gpio_expander.c 

OBJS += \
./Componets/GPIO\ expander/Src/gpio_expander.o 

C_DEPS += \
./Componets/GPIO\ expander/Src/gpio_expander.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/GPIO\ expander/Src/gpio_expander.o: ../Componets/GPIO\ expander/Src/gpio_expander.c Componets/GPIO\ expander/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Core/Inc/u8g2" -I"D:/VEGA/Git Hub/BMS Tester Fimware/BMS Tester - Cell Emulator/Componets/GPIO expander/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Componets/GPIO expander/Src/gpio_expander.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-GPIO-20-expander-2f-Src

clean-Componets-2f-GPIO-20-expander-2f-Src:
	-$(RM) ./Componets/GPIO\ expander/Src/gpio_expander.cyclo ./Componets/GPIO\ expander/Src/gpio_expander.d ./Componets/GPIO\ expander/Src/gpio_expander.o ./Componets/GPIO\ expander/Src/gpio_expander.su

.PHONY: clean-Componets-2f-GPIO-20-expander-2f-Src

