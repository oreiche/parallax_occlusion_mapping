cmake_minimum_required(VERSION 3.18)
project(ParallaxOcclusionMapping CXX)
enable_testing()

if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(externals)
include(external/gsl_lite)
include(external/catch2)
include(external/glew)
include(external/glfw)
include(external/glm)
include(external/freeimage)

if (NOT WIN32)
    # Convenience symlink to externals stage directory from source directory
    file(CREATE_LINK "${EXTERNALS_STAGE_DIR}" "${PROJECT_SOURCE_DIR}/.externals"
        SYMBOLIC)
else()
    file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR}/.externals LINK_PATH)
    file(TO_NATIVE_PATH ${EXTERNALS_STAGE_DIR} TARGET_PATH)
    message(NOTICE
        "For convenience, you might want to symlink externals to your source "
        "dir via running the following command in an elevated Powershell:\n"
        "    New-Item -ItemType SymbolicLink -Path \"${LINK_PATH}\" -Target \"${TARGET_PATH}\"")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wsign-conversion -Werror -pedantic)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)