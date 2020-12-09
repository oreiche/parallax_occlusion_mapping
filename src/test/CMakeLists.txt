option(OPENGL_TESTS "Enable OpenGL for tests" ON)
option(RUN_MEMCHECK "Run tests with memcheck" ON)
option(RUN_ANALYSIS "Run code analysis tests" OFF)

if (OPENGL_TESTS)
    add_definitions(-DWITH_OPENGL)
endif()

if (RUN_MEMCHECK)
    # Find valgrind
    find_program(CXX_MEMCHECK valgrind)
    set(CXX_MEMCHECK_OPTIONS --error-exitcode=1 --leak-check=full)
endif()

if (RUN_ANALYSIS)
    # Run static code analysis with bash
    find_program(BASH_EXE bash REQUIRED)

    if (WIN32)
        set(EXPORTS "export WIN32_EXT=.exe;")
    elseif (llvm_toolchain_ROOT)
        # if external llvm_toolchain is used
        set(EXPORTS "export PATH=\"${llvm_toolchain_ROOT}/bin\":$PATH;")
    endif()

    add_test(NAME code_analysis_clang-format
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND ${BASH_EXE} -c "${EXPORTS} .github/run-clang-format.sh src/ -v -e test.*data")
    add_test(NAME code_analysis_clang-tidy
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND ${BASH_EXE} -c "${EXPORTS} .github/run-clang-tidy.sh src/ -v -e test.*data")
endif()

# Helper macro creating tests with memcheck
macro (add_memchk_test TEST_NAME TEST_TARGET)
    add_test(NAME ${TEST_NAME}
             COMMAND $<TARGET_FILE:${TEST_TARGET}> ${ARGN}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    if (CXX_MEMCHECK)
        add_test(NAME ${TEST_NAME}_memchk
                 COMMAND ${CXX_MEMCHECK} ${CXX_MEMCHECK_OPTIONS}
                         $<TARGET_FILE:${TEST_TARGET}> ${ARGN}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif ()
endmacro ()

add_subdirectory(utils)
add_subdirectory(pom)