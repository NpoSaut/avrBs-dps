################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
bs-dps.lss \

FLASH_IMAGE += \
bs-dps.hex \

EEPROM_IMAGE += \
bs-dps.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: bs-dps.elf secondary-outputs

# Tool invocations
bs-dps.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,bs-dps.map,--cref -ffunction-sections -Wl,-gc-sections -T "../ld-script.x" -mmcu=at90can64 -o"bs-dps.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

bs-dps.lss: bs-dps.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S bs-dps.elf  >"bs-dps.lss"
	@echo 'Finished building: $@'
	@echo ' '

bs-dps.hex: bs-dps.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex bs-dps.elf  "bs-dps.hex"
	@echo 'Finished building: $@'
	@echo ' '

bs-dps.eep: bs-dps.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex bs-dps.elf  "bs-dps.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: bs-dps.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=at90can64 bs-dps.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) bs-dps.elf
	-@echo ' '

post-build:
	-avr-objcopy -R .eeprom -O binary bs-dps.elf "C:\Program files\Saut\Programs\DPS_CAN.FLA"
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
