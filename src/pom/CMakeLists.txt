add_subdirectory(image_loader)
add_subdirectory(shader_compiler)
add_subdirectory(renderer)

add_executable(pom_main main.cpp)
target_link_libraries(pom_main github_glew github_glm image_loader shader_compiler renderer)
