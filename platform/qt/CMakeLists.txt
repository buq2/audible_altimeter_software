FIND_PACKAGE(Qt5 REQUIRED)
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
    Qt5::Widgets 
    Qt5::Concurrent
    axCore
    axDisplays
    )
