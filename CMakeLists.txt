cmake_minimum_required (VERSION 2.8.11)
PROJECT(audible C CXX)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Concurrent_INCLUDE_DIRS})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-fPIC)
ADD_DEFINITIONS(-std=c++14)
ADD_DEFINITIONS(-Wall -Wextra)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    )

QT5_WRAP_CPP(GUI_HH_MOC 
    mainwindow.hh
    )
QT5_WRAP_UI(GUI_UI_MOC 
    mainwindow.ui
    )

ADD_SUBDIRECTORY(ui)

ADD_EXECUTABLE(audible_gui
    main.cpp
    config.cpp config.hh
    sensors.cpp sensors.hh    
    mainwindow.cpp mainwindow.hh
    display_buffer.cpp display_buffer.hh
    fonts/fontlibrary.h fonts/fontlibrary.c
    ${GUI_UI_MOC}
    ${GUI_HH_MOC}
    )
    
TARGET_LINK_LIBRARIES(audible_gui
    ${Qt5Widgets_LIBRARIES}
    Ui
    )
