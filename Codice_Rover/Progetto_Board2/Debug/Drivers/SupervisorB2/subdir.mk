################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SupervisorB2/SupervisorB2.c \
../Drivers/SupervisorB2/rtGetNaN.c \
../Drivers/SupervisorB2/rt_nonfinite.c \
../Drivers/SupervisorB2/rt_zcfcn.c 

OBJS += \
./Drivers/SupervisorB2/SupervisorB2.o \
./Drivers/SupervisorB2/rtGetNaN.o \
./Drivers/SupervisorB2/rt_nonfinite.o \
./Drivers/SupervisorB2/rt_zcfcn.o 

C_DEPS += \
./Drivers/SupervisorB2/SupervisorB2.d \
./Drivers/SupervisorB2/rtGetNaN.d \
./Drivers/SupervisorB2/rt_nonfinite.d \
./Drivers/SupervisorB2/rt_zcfcn.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SupervisorB2/%.o Drivers/SupervisorB2/%.su Drivers/SupervisorB2/%.cyclo: ../Drivers/SupervisorB2/%.c Drivers/SupervisorB2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/.." -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SupervisorB2

clean-Drivers-2f-SupervisorB2:
	-$(RM) ./Drivers/SupervisorB2/SupervisorB2.cyclo ./Drivers/SupervisorB2/SupervisorB2.d ./Drivers/SupervisorB2/SupervisorB2.o ./Drivers/SupervisorB2/SupervisorB2.su ./Drivers/SupervisorB2/rtGetNaN.cyclo ./Drivers/SupervisorB2/rtGetNaN.d ./Drivers/SupervisorB2/rtGetNaN.o ./Drivers/SupervisorB2/rtGetNaN.su ./Drivers/SupervisorB2/rt_nonfinite.cyclo ./Drivers/SupervisorB2/rt_nonfinite.d ./Drivers/SupervisorB2/rt_nonfinite.o ./Drivers/SupervisorB2/rt_nonfinite.su ./Drivers/SupervisorB2/rt_zcfcn.cyclo ./Drivers/SupervisorB2/rt_zcfcn.d ./Drivers/SupervisorB2/rt_zcfcn.o ./Drivers/SupervisorB2/rt_zcfcn.su

.PHONY: clean-Drivers-2f-SupervisorB2

