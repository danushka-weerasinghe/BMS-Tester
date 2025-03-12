################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/FATFS\ SD/FATFS_SD.c 

OBJS += \
./Core/Inc/FATFS\ SD/FATFS_SD.o 

C_DEPS += \
./Core/Inc/FATFS\ SD/FATFS_SD.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/FATFS\ SD/FATFS_SD.o: ../Core/Inc/FATFS\ SD/FATFS_SD.c Core/Inc/FATFS\ SD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Core/Inc/U8g2 Display" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/pavank/Documents/STM32 WorkSpace/BMS Tester/Cell Emulator/Cell Emulator/Core/Inc/FATFS SD" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Inc/FATFS SD/FATFS_SD.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-FATFS-20-SD

clean-Core-2f-Inc-2f-FATFS-20-SD:
	-$(RM) ./Core/Inc/FATFS\ SD/FATFS_SD.cyclo ./Core/Inc/FATFS\ SD/FATFS_SD.d ./Core/Inc/FATFS\ SD/FATFS_SD.o ./Core/Inc/FATFS\ SD/FATFS_SD.su

.PHONY: clean-Core-2f-Inc-2f-FATFS-20-SD

