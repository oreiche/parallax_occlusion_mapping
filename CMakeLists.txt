cmake_minimum_required(VERSION 3.18)
project(ParallaxOcclusionMapping CXX)

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wsign-conversion -Werror -pedantic")
endif()

include(cmake/external/gsl_lite.cmake)

if (NOT WIN32)
    # Convenience symlink to stage directory from source directory
    file(CREATE_LINK "${EXTERNALS_STAGE_DIR}"
                     "${PROJECT_SOURCE_DIR}/.externals" SYMBOLIC)
else()
    message(NOTICE
            "For convenience, you might want to symlink externals to your "
            "source dir via:\n    mklink /d '${PROJECT_SOURCE_DIR}/.externals' "
            "'${EXTERNALS_STAGE_DIR}'")
endif()

add_subdirectory(cpp)