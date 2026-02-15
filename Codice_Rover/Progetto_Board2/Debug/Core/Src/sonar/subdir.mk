################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sonar/sonar_hw.c \
../Core/Src/sonar/sonar_task.c 

OBJS += \
./Core/Src/sonar/sonar_hw.o \
./Core/Src/sonar/sonar_task.o 

C_DEPS += \
./Core/Src/sonar/sonar_hw.d \
./Core/Src/sonar/sonar_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sonar/%.o Core/Src/sonar/%.su Core/Src/sonar/%.cyclo: ../Core/Src/sonar/%.c Core/Src/sonar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/.." -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sonar

clean-Core-2f-Src-2f-sonar:
	-$(RM) ./Core/Src/sonar/sonar_hw.cyclo ./Core/Src/sonar/sonar_hw.d ./Core/Src/sonar/sonar_hw.o ./Core/Src/sonar/sonar_hw.su ./Core/Src/sonar/sonar_task.cyclo ./Core/Src/sonar/sonar_task.d ./Core/Src/sonar/sonar_task.o ./Core/Src/sonar/sonar_task.su

.PHONY: clean-Core-2f-Src-2f-sonar

