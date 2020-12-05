add_executable(shader_compiler.test shader_compiler.test.cpp)
target_link_libraries(shader_compiler.test PRIVATE test_main shader_compiler)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_memchk_test(shader_compiler shader_compiler.test)