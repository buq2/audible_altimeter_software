FIND_PACKAGE(Boost REQUIRED
    COMPONENTS 
    system
    thread
    )
FIND_PACKAGE(Qt5 REQUIRED)
FIND_PACKAGE(qwt REQUIRED)

QT5_WRAP_CPP(gui_moc 
            jump_viewer.hh
            )

ADD_EXECUTABLE(audible_log_fetcher
    main_log_fetcher.cpp
    log_fetcher.hh log_fetcher.cpp
    jump.hh jump.cpp
    jump_viewer.hh jump_viewer.cpp
    ${gui_moc}
    )
TARGET_LINK_LIBRARIES(audible_log_fetcher
    Boost::system
    Boost::thread
    Qt5::Widgets
    qwt::qwt
    )
