FIND_PACKAGE(Boost REQUIRED
    COMPONENTS 
    system
    thread
    )
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )
    
ADD_EXECUTABLE(audible_log_fetcher
    main_log_fetcher.cpp
    )
TARGET_LINK_LIBRARIES(audible_log_fetcher
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    )
