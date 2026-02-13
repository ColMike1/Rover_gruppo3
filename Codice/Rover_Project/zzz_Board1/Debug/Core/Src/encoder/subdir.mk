################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/encoder/encoder_hw.c \
../Core/Src/encoder/encoder_step.c 

OBJS += \
./Core/Src/encoder/encoder_hw.o \
./Core/Src/encoder/encoder_step.o 

C_DEPS += \
./Core/Src/encoder/encoder_hw.d \
./Core/Src/encoder/encoder_step.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/encoder/%.o Core/Src/encoder/%.su Core/Src/encoder/%.cyclo: ../Core/Src/encoder/%.c Core/Src/encoder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/OEM/STM32CubeIDE/workspace_1.19.0" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-encoder

clean-Core-2f-Src-2f-encoder:
	-$(RM) ./Core/Src/encoder/encoder_hw.cyclo ./Core/Src/encoder/encoder_hw.d ./Core/Src/encoder/encoder_hw.o ./Core/Src/encoder/encoder_hw.su ./Core/Src/encoder/encoder_step.cyclo ./Core/Src/encoder/encoder_step.d ./Core/Src/encoder/encoder_step.o ./Core/Src/encoder/encoder_step.su

.PHONY: clean-Core-2f-Src-2f-encoder

