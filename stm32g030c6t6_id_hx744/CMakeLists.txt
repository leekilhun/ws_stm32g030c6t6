cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")
include(arm-none-eabi-gcc)

set(PRJ_NAME stm32g030c6t6_id_hx744)

project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE ${PRJ_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/STM32G030C6TX_FLASH.ld)


file(GLOB SRC_FILES CONFIGURE_DEPENDS
  App/hw/*.c
  App/hw/driver/*.c
  App/exhw/*.c
  App/exhw/driver/*.c
  App/bsp/*.c
  Core/Src/main.cpp
  Core/Src/stm32g0xx_hal_msp.c
  Core/Src/stm32g0xx_it.c
  Core/Src/syscalls.c
  Core/Src/sysmem.c
  Core/Src/system_stm32g0xx.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
App/*.cpp
App/ap/*.cpp
App/ap/*.c
App/common/*.c    
Drivers/STM32G0xx_HAL_Driver/Src/*.c
  #lib/rtos/*.c
)

add_executable(${EXECUTABLE} 
  Core/Src/main.cpp
  Core/Startup/startup_stm32g030c6tx.s

  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32G030xx
  #-D_USE_HW_RTOS
  -DUSER_VECT_TAB_ADDRESS
  -DDEBUG
  )

target_include_directories(${EXECUTABLE} PRIVATE 
  App  
  App/ap
  App/ap/_inc
  App/bsp
  App/common 
  App/common/core
  App/common/inc_hw
  App/common/inc_exhw
  App/exhw
  App/hw
  Core/Inc
  
  Drivers/CMSIS/Device/ST/STM32G0xx/Include
  Drivers/CMSIS/Include
  Drivers/STM32G0xx_HAL_Driver/Inc

  #lib/rtos
  #lib/rtos/Source/CMSIS_RTOS
  #lib/rtos/Source/portable/GCC/ARM_CM0
  #lib/rtos/Source/include
)



target_compile_options(${EXECUTABLE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
    -mcpu=cortex-m0plus 
    -std=gnu++17 
    -g3 
    -Os 
    -ffunction-sections 
    -fdata-sections 
    -fno-exceptions 
    -fno-rtti 
    -fno-use-cxa-atexit 
    -Wall 
    -fstack-usage 
    --specs=nano.specs 
    -mfloat-abi=soft 
    -mthumb
    >
    )

target_compile_options(${EXECUTABLE} PRIVATE $<$<COMPILE_LANGUAGE:C>:
    -mcpu=cortex-m0plus 
    -std=gnu11 
    -g3 
    -Os 
    -ffunction-sections 
    -fdata-sections 
    -Wall 
    -fstack-usage 
    --specs=nano.specs 
    -mfloat-abi=soft 
    -mthumb
    >
    )  
  

target_link_options(${EXECUTABLE} PRIVATE
-T${LINKER_FILE}
-mcpu=cortex-m0plus

-mthumb
#-mfpu=fpv4-sp-d16
-mfloat-abi=soft
-specs=nano.specs
-specs=nosys.specs 
-lc
-lm
-Wl,-Map=${PROJECT_NAME}.map,--cref
-Wl,--gc-sections
-Xlinker -print-memory-usage -Xlinker
)


add_custom_command(TARGET ${EXECUTABLE} 
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
    COMMENT "Invoking: Make Binary"
    )

#set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")
#add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#        COMMAND arm-none-eabi-size --format=berkeley --totals "$<TARGET_FILE:${EXECUTABLE}>"
#        COMMENT "Invoking: Cross ARM GNU Print Size")

