################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SupervisorB1/SupervisorB1.c \
../Drivers/SupervisorB1/rtGetNaN.c \
../Drivers/SupervisorB1/rt_nonfinite.c 

OBJS += \
./Drivers/SupervisorB1/SupervisorB1.o \
./Drivers/SupervisorB1/rtGetNaN.o \
./Drivers/SupervisorB1/rt_nonfinite.o 

C_DEPS += \
./Drivers/SupervisorB1/SupervisorB1.d \
./Drivers/SupervisorB1/rtGetNaN.d \
./Drivers/SupervisorB1/rt_nonfinite.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SupervisorB1/%.o Drivers/SupervisorB1/%.su Drivers/SupervisorB1/%.cyclo: ../Drivers/SupervisorB1/%.c Drivers/SupervisorB1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/OEM/STM32CubeIDE/workspace_1.19.0" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SupervisorB1

clean-Drivers-2f-SupervisorB1:
	-$(RM) ./Drivers/SupervisorB1/SupervisorB1.cyclo ./Drivers/SupervisorB1/SupervisorB1.d ./Drivers/SupervisorB1/SupervisorB1.o ./Drivers/SupervisorB1/SupervisorB1.su ./Drivers/SupervisorB1/rtGetNaN.cyclo ./Drivers/SupervisorB1/rtGetNaN.d ./Drivers/SupervisorB1/rtGetNaN.o ./Drivers/SupervisorB1/rtGetNaN.su ./Drivers/SupervisorB1/rt_nonfinite.cyclo ./Drivers/SupervisorB1/rt_nonfinite.d ./Drivers/SupervisorB1/rt_nonfinite.o ./Drivers/SupervisorB1/rt_nonfinite.su

.PHONY: clean-Drivers-2f-SupervisorB1

