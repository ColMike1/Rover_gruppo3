################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/log/log_format.c \
../Core/Src/log/log_task.c \
../Core/Src/log/wcet_monitor.c 

OBJS += \
./Core/Src/log/log_format.o \
./Core/Src/log/log_task.o \
./Core/Src/log/wcet_monitor.o 

C_DEPS += \
./Core/Src/log/log_format.d \
./Core/Src/log/log_task.d \
./Core/Src/log/wcet_monitor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/log/%.o Core/Src/log/%.su Core/Src/log/%.cyclo: ../Core/Src/log/%.c Core/Src/log/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Desktop/STM32/workspace/Rover_Project_Fake/zzz_Board2" -I"C:/Users/Sterm/Desktop/STM32/workspace/Rover_Project_Fake" -I"C:/Users/Sterm/Desktop/STM32/workspace/Rover_Project_Fake/shared_header/shared_resources" -I"C:/Users/Sterm/Desktop/STM32/workspace/Rover_Project_Fake/shared_header" -I"C:/Users/Sterm/Desktop/STM32/workspace/Rover_Project_Fake/zzz_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-log

clean-Core-2f-Src-2f-log:
	-$(RM) ./Core/Src/log/log_format.cyclo ./Core/Src/log/log_format.d ./Core/Src/log/log_format.o ./Core/Src/log/log_format.su ./Core/Src/log/log_task.cyclo ./Core/Src/log/log_task.d ./Core/Src/log/log_task.o ./Core/Src/log/log_task.su ./Core/Src/log/wcet_monitor.cyclo ./Core/Src/log/wcet_monitor.d ./Core/Src/log/wcet_monitor.o ./Core/Src/log/wcet_monitor.su

.PHONY: clean-Core-2f-Src-2f-log

