include(FetchContent)

# Get GLFW from GitHub
FetchContent_Declare(
    github_glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        3.3.2)
FetchContent_MakeAvailable(github_glfw)

# Create specific target used by us
add_library(github_glfw INTERFACE)
target_link_libraries(github_glfw INTERFACE glfw)
target_include_directories(github_glfw INTERFACE
                           ${github_glfw_SOURCE_DIR}/include)
