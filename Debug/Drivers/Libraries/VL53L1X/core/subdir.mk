################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Libraries/VL53L1X/core/VL53L1X_api.c \
../Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.c 

OBJS += \
./Drivers/Libraries/VL53L1X/core/VL53L1X_api.o \
./Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.o 

C_DEPS += \
./Drivers/Libraries/VL53L1X/core/VL53L1X_api.d \
./Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Libraries/VL53L1X/core/VL53L1X_api.o: ../Drivers/Libraries/VL53L1X/core/VL53L1X_api.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/Libraries/DoorSensor -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Libraries/VL53L1X/core/VL53L1X_api.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.o: ../Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/Libraries/DoorSensor -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Libraries/VL53L1X/core/VL53L1X_calibration.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

