################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Inc/u8g2/subdir.mk
-include Componets/SD\ Card/Src/subdir.mk
-include Componets/RTC/Src/subdir.mk
-include Componets/RS485/Src/subdir.mk
-include Componets/ISO\ SPI/Src/subdir.mk
-include Componets/INA229/Src/subdir.mk
-include Componets/INA229/Inc/subdir.mk
-include Componets/GPIO\ expander/Src/subdir.mk
-include Componets/EEPROM/Src/subdir.mk
-include Componets/Display/Src/subdir.mk
-include Componets/Digital\ Pot/Src/subdir.mk
-include Componets/CAN/Src/subdir.mk
-include Componets/Button\ &\ LED/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := BMS Tester - Cell Emulator
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BMS\ Tester\ -\ Cell\ Emulator.elf \

MAP_FILES += \
BMS\ Tester\ -\ Cell\ Emulator.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BMS\ Tester\ -\ Cell\ Emulator.list \


# All Target
all: main-build

# Main-build Target
main-build: BMS\ Tester\ -\ Cell\ Emulator.elf secondary-outputs

# Tool invocations
BMS\ Tester\ -\ Cell\ Emulator.elf BMS\ Tester\ -\ Cell\ Emulator.map: $(OBJS) $(USER_OBJS) D:\VEGA\Git\ Hub\BMS\ Tester\ Fimware\BMS\ Tester\ -\ Cell\ Emulator\STM32F429IGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "BMS Tester - Cell Emulator.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\VEGA\Git Hub\BMS Tester Fimware\BMS Tester - Cell Emulator\STM32F429IGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BMS Tester - Cell Emulator.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BMS\ Tester\ -\ Cell\ Emulator.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BMS Tester - Cell Emulator.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) BMS\ Tester\ -\ Cell\ Emulator.elf BMS\ Tester\ -\ Cell\ Emulator.list BMS\ Tester\ -\ Cell\ Emulator.map BMS\\ Tester\\ -\\ Cell\\ Emulator.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
