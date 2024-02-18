#此文件从模板自动生成! 请勿更改!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(STM32_Demo C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Core/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
        Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
        Middlewares/Third_Party/FreeRTOS/Source/include
        Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        IST8310_I2C
        BMI088
        Mahony
        )
###################################################
#引入头文件
include_directories(Breathe_light)
include_directories(Buzzer)
include_directories(Steer_Engine)
include_directories(Interrupt_trigger)
include_directories(Serial_port)
include_directories(Direct_Memory_Access)
include_directories(Flash)
include_directories(3508_motor)
include_directories(PID)
include_directories(SPI_DMA)
include_directories(IMU_Slove)
include_directories(Board_Communication)
#引入源文件
set(Breathe_light Breathe_light/Breathe_Light.c)
set(Buzzer Buzzer/Buzzer.c)
set(Steer_Engine Steer_Engine/Steer_Engine.c)
set(Interrupt_trigger Interrupt_trigger/Interrupt_trigger.c)
set(Serial_port Serial_port/Serial_port.c)
set(Direct_Memory_Access Direct_Memory_Access/Direct_Memory_Access.c Direct_Memory_Access/remote_control.c)
set(Flash Flash/Flash.c )
set(3508_motor 3508_motor/Motor_CAN.c)
set(PID PID/PID.c)
set(SPI_DMA SPI_DMA/SPI_DMA.c)
set(IMU_Slove IMU_Slove/IMU_Slove.c)
set(Board_Communication Board_Communication/Board_communication.c)
###################################################
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx)

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*" "Middlewares/*.*" "IST8310_I2C/*.*" "BMI088/*.*" "Mahony/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407IGHX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT}
           ${Breathe_light}
           ${Buzzer}
           ${Steer_Engine}
           ${Interrupt_trigger}
           ${Serial_port}
           ${Direct_Memory_Access}
           ${Flash}
           ${3508_motor}
           ${PID}
           ${SPI_DMA}
           ${IMU_Slove}
           ${Board_Communication}
)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
