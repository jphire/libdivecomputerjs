include(GNUInstallDirs)

add_library(DiveComputer SHARED IMPORTED)

IF(NOT WIN32)
    include(ExternalProject)
    ExternalProject_Add(SetupLibDiveComputer
        URL https://github.com/libdivecomputer/libdivecomputer/releases/download/v0.8.0/libdivecomputer-0.8.0.tar.gz
        CONFIGURE_COMMAND autoreconf --install && ./configure --prefix=${CMAKE_BINARY_DIR}
        BUILD_IN_SOURCE TRUE
        BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/lib/libdivecomputer.so.0 ${CMAKE_BINARY_DIR}/lib/libdivecomputer.so
    )
    set (LIBDIVECOMPUTER_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
    set (LIBDIVECOMPUTER_LIBRARY ${CMAKE_BINARY_DIR}/lib)

    add_custom_command(
        TARGET SetupLibDiveComputer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -f
        ${LIBDIVECOMPUTER_LIBRARY}/libdivecomputer.so
    )
    add_custom_command(
        TARGET SetupLibDiveComputer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rename
        ${LIBDIVECOMPUTER_LIBRARY}/libdivecomputer.so.0.0.0
        ${LIBDIVECOMPUTER_LIBRARY}/libdivecomputer.so.0
    )
    set_target_properties(
        DiveComputer
        PROPERTIES
            IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libdivecomputer.so.0
        )
ENDIF()

IF(WIN32)

    set (LIBDIVECOMPUTER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dev/include)
    set (LIBDIVECOMPUTER_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/dev/bin)

    add_custom_target(SetupLibDiveComputer)
    set_property(TARGET DiveComputer PROPERTY IMPORTED_LOCATION "${LIBDIVECOMPUTER_LIBRARY}/libdivecomputer-0.dll")
    set_property(TARGET DiveComputer PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dev/def/libdivecomputer-0.lib")
    set_target_properties(DiveComputer PROPERTIES LINKER_LANGUAGE C)
    set_target_properties(DiveComputer PROPERTIES BIN_FILES "${LIBDIVECOMPUTER_LIBRARY}")
ENDIF()

add_dependencies(DiveComputer SetupLibDiveComputer)