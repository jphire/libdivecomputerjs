
IF(NOT WIN32)
    include(ExternalProject)  
    ExternalProject_Add(libdivecomputer
        URL https://github.com/libdivecomputer/libdivecomputer/archive/refs/heads/master.zip
        CONFIGURE_COMMAND autoreconf --install && ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/deps/libdivecomputer
        BUILD_IN_SOURCE TRUE
    )
ENDIF()

IF(WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dev/headers DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
    add_custom_target(libdivecomputer
        COMMAND node ${CMAKE_CURRENT_SOURCE_DIR}/dev/download-for-win32.js ${CMAKE_CURRENT_SOURCE_DIR}/deps
    )
ENDIF()

include_directories(${CMAKE_SOURCE_DIR}/deps/libdivecomputer/include)
link_directories(${CMAKE_SOURCE_DIR}/deps/libdivecomputer/lib)