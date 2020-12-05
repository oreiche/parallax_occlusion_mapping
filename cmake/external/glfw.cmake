# Get GLFW from GitHub
http_archive(
    NAME    github_glfw
    URL     "https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"
    SHA256  08a33a512f29d7dbf78eab39bd7858576adcc95228c9efe8e4bc5f0f3261efc7
    STRIP   "glfw-3.3.2")

# Add targets from GLFW
add_subdirectory(${github_glfw_ROOT}
                 ${github_glfw_ROOT}/.build EXCLUDE_FROM_ALL)

# Create specific target used by us
add_library(github_glfw INTERFACE)
target_link_libraries(github_glfw INTERFACE glfw)
target_include_directories(github_glfw INTERFACE ${github_glfw_ROOT}/include)
