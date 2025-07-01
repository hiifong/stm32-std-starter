cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
set(Defines_Syms
	USE_STDPERIPH_DRIVER
	STM32F10X_MD
    $<$<CONFIG:Debug>:DEBUG>
)

set(Include_Dirs
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Inc
)

set(Application_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f10x_it.c
    ${CMAKE_SOURCE_DIR}/Core/Src/core_cm3.c
    ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s
)

set(STM32_Drivers_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32f10x.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/misc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_adc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_bkp.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_can.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_cec.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_crc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_dac.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_dbgmcu.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_exti.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_flash.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_fsmc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_i2c.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_iwdg.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_pwr.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_rcc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_rtc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_sdio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_spi.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_tim.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_usart.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Src/stm32f10x_wwdg.c
)

# Drivers Midllewares



# Link directories setup
set(LINK_DIRS

)
# Project static libraries
set(LINK_LIBS
    STM32_Drivers

)
# Interface library for includes and symbols
add_library(stm32std INTERFACE)
target_include_directories(stm32std INTERFACE ${Include_Dirs})
target_compile_definitions(stm32std INTERFACE ${Defines_Syms})

# Create STM32_Drivers static library
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32std)


# Add application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate the code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "the code requires C11 or higher")
endif()
