# Get GLEW from GitHub
http_archive(
    NAME    github_glew
    URL     "https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz"
    SHA256  04de91e7e6763039bc11940095cd9c7f880baba82196a7765f727ac05a993c95
    STRIP   "glew-2.1.0")

# Add targets from GLEW
set(OpenGL_GL_PREFERENCE GLVND)
add_subdirectory(${github_glew_ROOT}/build/cmake
                 ${github_glew_ROOT}/.build EXCLUDE_FROM_ALL)

# Create specific target used by us
add_library(github_glew INTERFACE)
target_link_libraries(github_glew INTERFACE glew_s)
target_include_directories(github_glew INTERFACE ${github_glew_ROOT}/include)
target_compile_definitions(github_glew INTERFACE -DGLEW_NO_GLU)
