add_library(shader_compiler STATIC shader_compiler.cpp)
target_link_libraries(shader_compiler
    PRIVATE github_gsl_lite
    PUBLIC github_glew)