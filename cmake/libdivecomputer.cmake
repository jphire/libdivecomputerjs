
IF(NOT WIN32)
    include(ExternalProject)  
    ExternalProject_Add(libdivecomputer
        URL https://github.com/libdivecomputer/libdivecomputer/archive/refs/heads/master.zip
        CONFIGURE_COMMAND autoreconf --install && ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/deps/
        BUILD_IN_SOURCE TRUE
    )
ENDIF()

IF(WIN32)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dev/libdivecomputer DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
    add_custom_target(libdivecomputer
        COMMAND node ${CMAKE_CURRENT_SOURCE_DIR}/dev/download-for-win32.js ${CMAKE_CURRENT_SOURCE_DIR}/deps
    )
    add_library(divecomputer SHARED IMPORTED)
    set_property(TARGET divecomputer PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/libdivecomputer-0.dll")
    set_property(TARGET divecomputer PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/libdivecomputer-0.lib")
    set_target_properties(divecomputer PROPERTIES LINKER_LANGUAGE C)
ENDIF()

include_directories(${CMAKE_SOURCE_DIR}/deps/include)
link_directories(${CMAKE_SOURCE_DIR}/deps/lib)