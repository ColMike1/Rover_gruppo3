################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/supervisor/supervisor_task.c 

OBJS += \
./Core/Src/supervisor/supervisor_task.o 

C_DEPS += \
./Core/Src/supervisor/supervisor_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/supervisor/%.o Core/Src/supervisor/%.su Core/Src/supervisor/%.cyclo: ../Core/Src/supervisor/%.c Core/Src/supervisor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover/Progetto_Board2" -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover" -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover/Progetto_Board2/.." -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-supervisor

clean-Core-2f-Src-2f-supervisor:
	-$(RM) ./Core/Src/supervisor/supervisor_task.cyclo ./Core/Src/supervisor/supervisor_task.d ./Core/Src/supervisor/supervisor_task.o ./Core/Src/supervisor/supervisor_task.su

.PHONY: clean-Core-2f-Src-2f-supervisor

