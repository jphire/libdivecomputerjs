cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0015 NEW)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

project (libdivecomputerjs)

include(cmake/libdivecomputer.cmake)

file(GLOB_RECURSE SOURCE_FILES src/*.cc)

set_directory_properties(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${CMAKE_JS_INC})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
add_dependencies(${PROJECT_NAME} libdivecomputer)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./deps/lib")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} divecomputer)



IF(WIN32)
    get_target_property(bin_dir divecomputer BIN_FILES)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${bin_dir}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
ENDIF()

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

add_definitions(-DNAPI_VERSION=6)