FIND_PACKAGE(Boost REQUIRED
    COMPONENTS 
    system
    thread
    )
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )

FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

QT5_WRAP_CPP(gui_moc 
            jump_viewer.hh
            )

INCLUDE_DIRECTORIES(/usr/include/qwt/)

ADD_EXECUTABLE(audible_log_fetcher
    main_log_fetcher.cpp
    log_fetcher.hh log_fetcher.cpp
    jump.hh jump.cpp
    jump_viewer.hh jump_viewer.cpp
    ${gui_moc}
    )
TARGET_LINK_LIBRARIES(audible_log_fetcher
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Qt5Widgets_LIBRARIES}
    -lqwt
    )
