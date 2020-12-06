add_subdirectory(shader_compiler)
add_subdirectory(renderer)

add_executable(pom_main main.cpp)
target_link_libraries(pom_main github_glew github_glm github_freeimageplus shader_compiler renderer)
