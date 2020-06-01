################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/Libraries/VL53L1X/platform/subdir.mk
-include Drivers/Libraries/VL53L1X/core/subdir.mk
-include Drivers/Libraries/DoorSensor/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PC_Proto_2.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
PC_Proto_2.list \

OBJCOPY_BIN += \
PC_Proto_2.bin \


# All Target
all: PC_Proto_2.elf secondary-outputs

# Tool invocations
PC_Proto_2.elf: $(OBJS) $(USER_OBJS) C:\Asus\ WebStorage\diegobailos@hotmail.com\MySyncFolder\Proyectos\ DIY\Contador\ de\ Personas\Proto_2\STM32L432KCUX_FLASH.ld
	arm-none-eabi-gcc -o "PC_Proto_2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Asus WebStorage\diegobailos@hotmail.com\MySyncFolder\Proyectos DIY\Contador de Personas\Proto_2\STM32L432KCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="PC_Proto_2.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

PC_Proto_2.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "PC_Proto_2.list"
	@echo 'Finished building: $@'
	@echo ' '

PC_Proto_2.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "PC_Proto_2.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
