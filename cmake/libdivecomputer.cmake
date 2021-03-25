
include(ExternalProject)

ExternalProject_Add(libdivecomputer
    URL https://github.com/libdivecomputer/libdivecomputer/archive/refs/heads/master.zip
    CONFIGURE_COMMAND autoreconf --install && ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/deps/libdivecomputer
    BUILD_IN_SOURCE TRUE
)
include_directories(${CMAKE_SOURCE_DIR}/deps/libdivecomputer/include)
link_directories(${CMAKE_SOURCE_DIR}/deps/libdivecomputer/lib)