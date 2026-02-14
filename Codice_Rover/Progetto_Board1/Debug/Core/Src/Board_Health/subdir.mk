################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Board_Health/board_health_adc.c \
../Core/Src/Board_Health/board_health_task.c 

OBJS += \
./Core/Src/Board_Health/board_health_adc.o \
./Core/Src/Board_Health/board_health_task.o 

C_DEPS += \
./Core/Src/Board_Health/board_health_adc.d \
./Core/Src/Board_Health/board_health_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Board_Health/%.o Core/Src/Board_Health/%.su Core/Src/Board_Health/%.cyclo: ../Core/Src/Board_Health/%.c Core/Src/Board_Health/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover/Progetto_Board1/.." -I"C:/Users/Sterm/Desktop/STM32/workspace/Codice_Rover/Progetto_Board1/Drivers/SupervisorB1" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Board_Health

clean-Core-2f-Src-2f-Board_Health:
	-$(RM) ./Core/Src/Board_Health/board_health_adc.cyclo ./Core/Src/Board_Health/board_health_adc.d ./Core/Src/Board_Health/board_health_adc.o ./Core/Src/Board_Health/board_health_adc.su ./Core/Src/Board_Health/board_health_task.cyclo ./Core/Src/Board_Health/board_health_task.d ./Core/Src/Board_Health/board_health_task.o ./Core/Src/Board_Health/board_health_task.su

.PHONY: clean-Core-2f-Src-2f-Board_Health

