################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../External_Libs/btn_manage/stm_button.c 

OBJS += \
./External_Libs/btn_manage/stm_button.o 

C_DEPS += \
./External_Libs/btn_manage/stm_button.d 


# Each subdirectory must supply rules for building sources it contributes
External_Libs/btn_manage/%.o External_Libs/btn_manage/%.su External_Libs/btn_manage/%.cyclo: ../External_Libs/btn_manage/%.c External_Libs/btn_manage/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-External_Libs-2f-btn_manage

clean-External_Libs-2f-btn_manage:
	-$(RM) ./External_Libs/btn_manage/stm_button.cyclo ./External_Libs/btn_manage/stm_button.d ./External_Libs/btn_manage/stm_button.o ./External_Libs/btn_manage/stm_button.su

.PHONY: clean-External_Libs-2f-btn_manage

