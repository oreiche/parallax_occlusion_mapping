list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(externals)

# Get GSL from GitHub
http_archive(
    NAME     github_gsl_lite
    URL      "https://github.com/gsl-lite/gsl-lite/archive/0.37.0.zip"
    SHA256   980c8e91dd25bfaac7bfbd7151f04027fa68ff0bf652baa11de137242641e399
    PATTERNS "gsl-lite-0.37.0/include"
    STRIP    "gsl-lite-0.37.0")

# Create header-only library
add_library(github_gsl_lite INTERFACE)
target_include_directories(github_gsl_lite
                           INTERFACE ${github_gsl_lite_ROOT}/include)
