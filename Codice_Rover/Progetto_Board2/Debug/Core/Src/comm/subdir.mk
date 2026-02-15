################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/comm/comm_pack.c \
../Core/Src/comm/comm_rx_task.c \
../Core/Src/comm/comm_tx_task.c \
../Core/Src/comm/comm_uart.c \
../Core/Src/comm/comm_unpack.c 

OBJS += \
./Core/Src/comm/comm_pack.o \
./Core/Src/comm/comm_rx_task.o \
./Core/Src/comm/comm_tx_task.o \
./Core/Src/comm/comm_uart.o \
./Core/Src/comm/comm_unpack.o 

C_DEPS += \
./Core/Src/comm/comm_pack.d \
./Core/Src/comm/comm_rx_task.d \
./Core/Src/comm/comm_tx_task.d \
./Core/Src/comm/comm_uart.d \
./Core/Src/comm/comm_unpack.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/comm/%.o Core/Src/comm/%.su Core/Src/comm/%.cyclo: ../Core/Src/comm/%.c Core/Src/comm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover" -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/.." -I"C:/Users/Sterm/Documents/GitHub/Rover_gruppo3/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-comm

clean-Core-2f-Src-2f-comm:
	-$(RM) ./Core/Src/comm/comm_pack.cyclo ./Core/Src/comm/comm_pack.d ./Core/Src/comm/comm_pack.o ./Core/Src/comm/comm_pack.su ./Core/Src/comm/comm_rx_task.cyclo ./Core/Src/comm/comm_rx_task.d ./Core/Src/comm/comm_rx_task.o ./Core/Src/comm/comm_rx_task.su ./Core/Src/comm/comm_tx_task.cyclo ./Core/Src/comm/comm_tx_task.d ./Core/Src/comm/comm_tx_task.o ./Core/Src/comm/comm_tx_task.su ./Core/Src/comm/comm_uart.cyclo ./Core/Src/comm/comm_uart.d ./Core/Src/comm/comm_uart.o ./Core/Src/comm/comm_uart.su ./Core/Src/comm/comm_unpack.cyclo ./Core/Src/comm/comm_unpack.d ./Core/Src/comm/comm_unpack.o ./Core/Src/comm/comm_unpack.su

.PHONY: clean-Core-2f-Src-2f-comm

