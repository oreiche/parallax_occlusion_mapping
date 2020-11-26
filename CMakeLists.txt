cmake_minimum_required(VERSION 3.18)
project(ParallaxOcclusionMapping CXX)

if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

include(cmake/external/gsl_lite.cmake)
include(cmake/external/glew.cmake)
include(cmake/external/glfw.cmake)

if (NOT WIN32)
    # Convenience symlink to stage directory from source directory
    file(CREATE_LINK "${EXTERNALS_STAGE_DIR}"
                     "${PROJECT_SOURCE_DIR}/.externals" SYMBOLIC)
else()
    file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR}/.externals LINK_PATH)
    file(TO_NATIVE_PATH ${EXTERNALS_STAGE_DIR} TARGET_PATH)
    message(NOTICE "For convenience, you might want to symlink externals to "
                   "your source dir via:\n"
                   "    mklink /d ${LINK_PATH} ${TARGET_PATH}")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-std=c++17
                        -Wall -Wextra -Wsign-conversion -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/std:c++17 /W4 /WX)
endif()

add_subdirectory(cpp)