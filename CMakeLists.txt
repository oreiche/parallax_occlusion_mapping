cmake_minimum_required(VERSION 3.18)
project(ParallaxOcclusionMapping CXX)

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wsign-conversion -Werror -pedantic")
endif()

include(cmake/external/gsl_lite.cmake)

add_subdirectory(cpp)