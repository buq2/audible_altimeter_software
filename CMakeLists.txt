cmake_minimum_required (VERSION 2.8.11)
PROJECT(audible C CXX)

OPTION(USE_MCU "If true, project will be compiled for MCU and not for AMD64 (qt)" OFF)
IF (USE_MCU)
    INCLUDE(platform/xmega/cmake_configuration.cmake)
ENDIF()

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Wall -Wextra)
ADD_DEFINITIONS(-fshort-enums)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    )

ADD_LIBRARY(Common STATIC
    common.hh common.cpp
    )

INCLUDE_DIRECTORIES(platform/xmega/xmega-libraries)
ADD_SUBDIRECTORY(ui)
ADD_SUBDIRECTORY(platform)

ADD_EXECUTABLE(audible
    common.hh
    config.cpp config.hh
    sensors.cpp sensors.hh
    #display_buffer.cpp display_buffer.hh
    #fonts/fontlibrary.h fonts/fontlibrary.c
    )
    
TARGET_LINK_LIBRARIES(audible
    Platform
    Ui
    Common
    )
