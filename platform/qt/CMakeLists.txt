FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Concurrent_INCLUDE_DIRS})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-fPIC)

QT5_WRAP_CPP(GUI_HH_MOC 
    mainwindow.hh
    )
QT5_WRAP_UI(GUI_UI_MOC 
    mainwindow.ui
    )

ADD_LIBRARY(Platform STATIC
    main_qt.cpp
    mainwindow.cpp mainwindow.hh
    ${GUI_UI_MOC}
    ${GUI_HH_MOC}
    )

TARGET_LINK_LIBRARIES(Platform
    ${Qt5Widgets_LIBRARIES}
    )
