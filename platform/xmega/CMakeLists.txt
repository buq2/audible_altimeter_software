INCLUDE(cmake_configuration.cmake)
INCLUDE_DIRECTORIES("lufa_config/")
INCLUDE_DIRECTORIES(${LUFA_INCLUDE_DIRECTORIES})
ADD_SUBDIRECTORY("lufa_config")
ADD_SUBDIRECTORY(xmega-libraries)
INCLUDE_DIRECTORIES(xmega-libraries)

FILE(GLOB LUFA_USB_C
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Class/Device/
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Class/Device/*)
FILE(GLOB LUFA_USB_C_CORE
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Core/
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Core/*.c)
FILE(GLOB_RECURSE LUFA_USB_C_PLATFORM
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Core/${MCU_ARCH}/
    ${LUFA_LOCATION}/LUFA/Drivers/USB/Core/${MCU_ARCH}/*.c)

FILE(GLOB LUFA_PERIPHERAL
    ${LUFA_LOCATION}/LUFA/Drivers/Peripheral/${MCU_ARCH}/
    ${LUFA_LOCATION}/LUFA/Drivers/Peripheral/${MCU_ARCH}/*.c)

ADD_LIBRARY(lufa STATIC
    ${LUFA_USB_C}
    ${LUFA_USB_C_CORE}
    ${LUFA_USB_C_PLATFORM}
    ${LUFA_PERIPHERAL}
    )

ADD_LIBRARY(Platform STATIC
    buttons.hh buttons.cpp
    timer.hh timer.cpp
    memory.hh memory.cpp
    sensor_controller.hh sensor_controller.cpp
    main_mcu.cpp
    )

TARGET_LINK_LIBRARIES(Platform
    LufaConfig
    axSensors
    axDisplays
    axMemory
    lufa
    )
