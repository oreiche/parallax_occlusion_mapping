# Main for all tests
add_library(test_main test_main.cpp)
target_link_libraries(test_main PUBLIC github_catch2 github_glew github_glfw)