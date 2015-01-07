INCLUDE(cmake_configuration.cmake)
INCLUDE_DIRECTORIES("lufa_config/")
INCLUDE_DIRECTORIES(${LUFA_INCLUDE_DIRECTORIES})
ADD_SUBDIRECTORY("lufa_config")

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

ADD_LIBRARY(Platform STATIC
    main_mcu.cpp
    display_sharp.hh display_sharp.cpp
    altimeter_mpl3115a2.hh altimeter_mpl3115a2.cpp
    altimeter_ms5805_02ba01.hh altimeter_ms5805_02ba01.cpp
    ${LUFA_USB_C}
    ${LUFA_USB_C_CORE}
    ${LUFA_USB_C_PLATFORM}
    ${LUFA_PERIPHERAL}
    )

TARGET_LINK_LIBRARIES(Platform
    LufaConfig
    )
