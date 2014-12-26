cmake_minimum_required (VERSION 2.8.11)
PROJECT(audible C CXX)

ADD_DEFINITIONS(-std=c++14)
ADD_DEFINITIONS(-Wall -Wextra)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    )

ADD_SUBDIRECTORY(ui)
ADD_SUBDIRECTORY(platform)

ADD_EXECUTABLE(audible
    config.cpp config.hh
    sensors.cpp sensors.hh
    display_buffer.cpp display_buffer.hh
    fonts/fontlibrary.h fonts/fontlibrary.c
    )
    
TARGET_LINK_LIBRARIES(audible
    Platform
    Ui
    )
