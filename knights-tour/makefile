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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PasseioDoCavalo

# Tool invocations
PasseioDoCavalo: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o"PasseioDoCavalo" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) PasseioDoCavalo
	-@echo ' '

run:	$(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "PasseioDoCavalo" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	./PasseioDoCavalo -n 3 -m 4 -i 1 -j 1 -e det -s saida.txt
	@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
