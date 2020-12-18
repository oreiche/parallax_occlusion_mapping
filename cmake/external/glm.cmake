include(FetchContent)

# Get GLM from GitHub
FetchContent_Declare(
    github_glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8)
FetchContent_MakeAvailable(github_glm)

# Create header-only library
add_library(github_glm INTERFACE)
target_include_directories(github_glm INTERFACE ${github_glm_SOURCE_DIR})
