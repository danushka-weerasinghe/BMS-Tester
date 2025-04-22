################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LED_BuiltIn/LED_BuiltIn.c 

OBJS += \
./LED_BuiltIn/LED_BuiltIn.o 

C_DEPS += \
./LED_BuiltIn/LED_BuiltIn.d 


# Each subdirectory must supply rules for building sources it contributes
LED_BuiltIn/%.o LED_BuiltIn/%.su LED_BuiltIn/%.cyclo: ../LED_BuiltIn/%.c LED_BuiltIn/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Core/Inc/U8g2 Display" -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/LED_BuiltIn" -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Switches" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LED_BuiltIn

clean-LED_BuiltIn:
	-$(RM) ./LED_BuiltIn/LED_BuiltIn.cyclo ./LED_BuiltIn/LED_BuiltIn.d ./LED_BuiltIn/LED_BuiltIn.o ./LED_BuiltIn/LED_BuiltIn.su

.PHONY: clean-LED_BuiltIn

