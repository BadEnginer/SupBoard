################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DAC_out/MCP4725.c 

OBJS += \
./Core/Src/DAC_out/MCP4725.o 

C_DEPS += \
./Core/Src/DAC_out/MCP4725.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DAC_out/%.o Core/Src/DAC_out/%.su Core/Src/DAC_out/%.cyclo: ../Core/Src/DAC_out/%.c Core/Src/DAC_out/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DAC_out

clean-Core-2f-Src-2f-DAC_out:
	-$(RM) ./Core/Src/DAC_out/MCP4725.cyclo ./Core/Src/DAC_out/MCP4725.d ./Core/Src/DAC_out/MCP4725.o ./Core/Src/DAC_out/MCP4725.su

.PHONY: clean-Core-2f-Src-2f-DAC_out

