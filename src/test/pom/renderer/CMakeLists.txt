add_executable(renderer.test renderer.test.cpp)
target_link_libraries(renderer.test PRIVATE test_main renderer)

add_memchk_test(renderer renderer.test)