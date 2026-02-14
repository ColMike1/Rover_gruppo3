################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BLE_Controller/ble_controller_i2c.c \
../Core/Src/BLE_Controller/ble_controller_task.c 

OBJS += \
./Core/Src/BLE_Controller/ble_controller_i2c.o \
./Core/Src/BLE_Controller/ble_controller_task.o 

C_DEPS += \
./Core/Src/BLE_Controller/ble_controller_i2c.d \
./Core/Src/BLE_Controller/ble_controller_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BLE_Controller/%.o Core/Src/BLE_Controller/%.su Core/Src/BLE_Controller/%.cyclo: ../Core/Src/BLE_Controller/%.c Core/Src/BLE_Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2" -I"C:/Users/miche/STM32CubeIDE/workspace_1.18.1" -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2/.." -I"C:/Users/miche/Desktop/GitRepositories/Rover_gruppo3/Codice_Rover/Progetto_Board2/Drivers/SupervisorB2" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-BLE_Controller

clean-Core-2f-Src-2f-BLE_Controller:
	-$(RM) ./Core/Src/BLE_Controller/ble_controller_i2c.cyclo ./Core/Src/BLE_Controller/ble_controller_i2c.d ./Core/Src/BLE_Controller/ble_controller_i2c.o ./Core/Src/BLE_Controller/ble_controller_i2c.su ./Core/Src/BLE_Controller/ble_controller_task.cyclo ./Core/Src/BLE_Controller/ble_controller_task.d ./Core/Src/BLE_Controller/ble_controller_task.o ./Core/Src/BLE_Controller/ble_controller_task.su

.PHONY: clean-Core-2f-Src-2f-BLE_Controller

