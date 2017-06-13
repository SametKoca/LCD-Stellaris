################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SW-EK-LM4F120XL-9453/utils/uartstdio.c 

OBJS += \
./SW-EK-LM4F120XL-9453/utils/uartstdio.o 

C_DEPS += \
./SW-EK-LM4F120XL-9453/utils/uartstdio.d 


# Each subdirectory must supply rules for building sources it contributes
SW-EK-LM4F120XL-9453/utils/%.o: ../SW-EK-LM4F120XL-9453/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/home/user/workspace/Prolab2Proje3/SW-EK-LM4F120XL-9453" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


