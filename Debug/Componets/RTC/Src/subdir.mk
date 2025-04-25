################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Componets/RTC/Src/rtc.c 

OBJS += \
./Componets/RTC/Src/rtc.o 

C_DEPS += \
./Componets/RTC/Src/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
Componets/RTC/Src/%.o Componets/RTC/Src/%.su Componets/RTC/Src/%.cyclo: ../Componets/RTC/Src/%.c Componets/RTC/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Core/Inc/u8g2" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/GPIO expander/Inc" -I"C:/Users/pavank/Documents/GitHub/BMS-Tester/Componets/RTC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Componets-2f-RTC-2f-Src

clean-Componets-2f-RTC-2f-Src:
	-$(RM) ./Componets/RTC/Src/rtc.cyclo ./Componets/RTC/Src/rtc.d ./Componets/RTC/Src/rtc.o ./Componets/RTC/Src/rtc.su

.PHONY: clean-Componets-2f-RTC-2f-Src

