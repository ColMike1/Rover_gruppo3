################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/actuation/actuation_step.c \
../Core/Src/actuation/sabertooth_driver.c 

OBJS += \
./Core/Src/actuation/actuation_step.o \
./Core/Src/actuation/sabertooth_driver.o 

C_DEPS += \
./Core/Src/actuation/actuation_step.d \
./Core/Src/actuation/sabertooth_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/actuation/%.o Core/Src/actuation/%.su Core/Src/actuation/%.cyclo: ../Core/Src/actuation/%.c Core/Src/actuation/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board1/.." -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board1/Drivers/SupervisorB1" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-actuation

clean-Core-2f-Src-2f-actuation:
	-$(RM) ./Core/Src/actuation/actuation_step.cyclo ./Core/Src/actuation/actuation_step.d ./Core/Src/actuation/actuation_step.o ./Core/Src/actuation/actuation_step.su ./Core/Src/actuation/sabertooth_driver.cyclo ./Core/Src/actuation/sabertooth_driver.d ./Core/Src/actuation/sabertooth_driver.o ./Core/Src/actuation/sabertooth_driver.su

.PHONY: clean-Core-2f-Src-2f-actuation

