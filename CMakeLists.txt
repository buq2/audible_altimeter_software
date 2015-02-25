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

INCLUDE_DIRECTORIES(platform/xmega/xmega-libraries)
ADD_SUBDIRECTORY(ui)
ADD_SUBDIRECTORY(platform)
IF (NOT USE_MCU)
    # Qt code requires some of the axlib stuff (core, displays)
    ADD_SUBDIRECTORY(platform/xmega/xmega-libraries)
ENDIF()

ADD_EXECUTABLE(audible
    config.cpp config.hh
    sensors.cpp sensors.hh
    )
    
TARGET_LINK_LIBRARIES(audible
    Platform
    Ui
    )
