include(FetchContent)

# Get Catch2 from GitHub
FetchContent_Declare(
    github_catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.3)
FetchContent_MakeAvailable(github_catch2)

# Create specific target used by us
add_library(github_catch2 INTERFACE)
target_include_directories(github_catch2 INTERFACE
                           ${github_catch2_SOURCE_DIR}/single_include)
