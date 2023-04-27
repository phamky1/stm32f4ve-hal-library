################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../External_Libs/stm_log/stm_log.c 

OBJS += \
./External_Libs/stm_log/stm_log.o 

C_DEPS += \
./External_Libs/stm_log/stm_log.d 


# Each subdirectory must supply rules for building sources it contributes
External_Libs/stm_log/%.o External_Libs/stm_log/%.su External_Libs/stm_log/%.cyclo: ../External_Libs/stm_log/%.c External_Libs/stm_log/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-External_Libs-2f-stm_log

clean-External_Libs-2f-stm_log:
	-$(RM) ./External_Libs/stm_log/stm_log.cyclo ./External_Libs/stm_log/stm_log.d ./External_Libs/stm_log/stm_log.o ./External_Libs/stm_log/stm_log.su

.PHONY: clean-External_Libs-2f-stm_log

