option(OPENGL_TESTS "Enable OpenGL for tests" ON)
option(RUN_MEMCHECK "Run tests with memcheck" ON)

if (OPENGL_TESTS)
    add_definitions(-DWITH_OPENGL)
endif()

if (RUN_MEMCHECK)
    # Find valgrind
    find_program(CXX_MEMCHECK valgrind)
    set(CXX_MEMCHECK_OPTIONS --error-exitcode=1 --leak-check=full)
endif()

# Helper macro creating tests with memcheck
macro (add_memchk_test TEST_NAME TEST_TARGET)
    add_test(NAME ${TEST_NAME}
             COMMAND $<TARGET_FILE:${TEST_TARGET}> ${ARGN}
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    if (CXX_MEMCHECK)
        add_test(NAME ${TEST_NAME}_memchk
                 COMMAND ${CXX_MEMCHECK} ${CXX_MEMCHECK_OPTIONS}
                         $<TARGET_FILE:${TEST_TARGET}> ${ARGN}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif ()
endmacro ()

# Main for all tests
add_library(test_main test_main.cpp)
target_link_libraries(test_main PUBLIC github_catch2 github_glew github_glfw)

# Tests
add_subdirectory(shader_compiler)
add_subdirectory(renderer)