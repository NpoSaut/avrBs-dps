################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp 

OBJS += \
./main.o 

CPP_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Programming\avr-cpp-libs\my" -I"D:\Programming\avr-cpp-libs\foreign" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fno-rtti -Wno-pmf-conversions -funsigned-char -funsigned-bitfields -fno-exceptions -fno-threadsafe-statics -std=c++0x -fconstexpr-depth=32768 -mmcu=at90can128 -DF_CPU=12000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


