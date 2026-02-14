################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/snapshot/ble_controller_snapshot.c \
../Core/Src/snapshot/imu_snapshot.c \
../Core/Src/snapshot/rx_snapshot.c \
../Core/Src/snapshot/sonar_snapshot.c \
../Core/Src/snapshot/supervisor_snapshot.c 

OBJS += \
./Core/Src/snapshot/ble_controller_snapshot.o \
./Core/Src/snapshot/imu_snapshot.o \
./Core/Src/snapshot/rx_snapshot.o \
./Core/Src/snapshot/sonar_snapshot.o \
./Core/Src/snapshot/supervisor_snapshot.o 

C_DEPS += \
./Core/Src/snapshot/ble_controller_snapshot.d \
./Core/Src/snapshot/imu_snapshot.d \
./Core/Src/snapshot/rx_snapshot.d \
./Core/Src/snapshot/sonar_snapshot.d \
./Core/Src/snapshot/supervisor_snapshot.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/snapshot/%.o Core/Src/snapshot/%.su Core/Src/snapshot/%.cyclo: ../Core/Src/snapshot/%.c Core/Src/snapshot/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2" -I"C:/Users/miche/STM32CubeIDE/workspace_1.18.1" -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2/.." -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-snapshot

clean-Core-2f-Src-2f-snapshot:
	-$(RM) ./Core/Src/snapshot/ble_controller_snapshot.cyclo ./Core/Src/snapshot/ble_controller_snapshot.d ./Core/Src/snapshot/ble_controller_snapshot.o ./Core/Src/snapshot/ble_controller_snapshot.su ./Core/Src/snapshot/imu_snapshot.cyclo ./Core/Src/snapshot/imu_snapshot.d ./Core/Src/snapshot/imu_snapshot.o ./Core/Src/snapshot/imu_snapshot.su ./Core/Src/snapshot/rx_snapshot.cyclo ./Core/Src/snapshot/rx_snapshot.d ./Core/Src/snapshot/rx_snapshot.o ./Core/Src/snapshot/rx_snapshot.su ./Core/Src/snapshot/sonar_snapshot.cyclo ./Core/Src/snapshot/sonar_snapshot.d ./Core/Src/snapshot/sonar_snapshot.o ./Core/Src/snapshot/sonar_snapshot.su ./Core/Src/snapshot/supervisor_snapshot.cyclo ./Core/Src/snapshot/supervisor_snapshot.d ./Core/Src/snapshot/supervisor_snapshot.o ./Core/Src/snapshot/supervisor_snapshot.su

.PHONY: clean-Core-2f-Src-2f-snapshot

