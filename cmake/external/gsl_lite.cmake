include(FetchContent)

# Get GSL from GitHub
FetchContent_Declare(
    github_gsl_lite
    GIT_REPOSITORY https://github.com/gsl-lite/gsl-lite.git
    GIT_TAG        v0.37.0)
FetchContent_MakeAvailable(github_gsl_lite)

# Create header-only library
add_library(github_gsl_lite INTERFACE)
target_include_directories(github_gsl_lite INTERFACE
                           ${github_gsl_lite_SOURCE_DIR}/include)
