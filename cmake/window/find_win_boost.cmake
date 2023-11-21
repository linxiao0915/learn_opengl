
# 添加boost
if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)  # only find static libs
    set(Boost_USE_DEBUG_LIBS OFF)  # ignore debug libs and
    set(Boost_USE_RELEASE_LIBS ON)  # only find release libs
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_NO_SYSTEM_PATHS ON)
endif ()
find_package(Boost 1.51.0 MODULE COMPONENTS system filesystem serialization locale iostreams date_time REQUIRED)
if (Boost_FOUND)
    if (WIN32)
        set(BOOST_INCLUDEDIR "${Boost_INCLUDE_DIRS}/include/boost-1_75")
        include_directories(${BOOST_INCLUDEDIR})
    endif ()
    link_directories(${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "The Boost lib is found!")
    message(STATUS "[cmake] Boost_LIBRARIES:             ${Boost_LIBRARIES}")
    message(STATUS "[cmake] BOOST_INCLUDEDIR:            ${BOOST_INCLUDEDIR}")
    message(STATUS "[cmake] Boost_INCLUDE_DIRS:          ${Boost_INCLUDE_DIRS}")
    message(STATUS "[cmake] BOOST_LINK_STATIC:           ${BOOST_LINK_STATIC}")
    message(STATUS "[cmake] Boost_LIBRARY_DIRS:          ${Boost_LIBRARY_DIRS}")
    message(STATUS "[cmake] Boost_VERSION:               ${Boost_VERSION}")
else ()
    message(STATUS "The Boost lib is no found!")
endif ()
