# Get Catch2 from GitHub
http_archive(
    NAME     github_catch2
    URL      "https://github.com/catchorg/Catch2/archive/v2.13.3.zip"
    SHA256   1804feb72bc15c0856b4a43aa586c661af9c3685a75973b6a8fc0b950c7cfd13
    PATTERNS "Catch2-2.13.3/single_include"
    STRIP    "Catch2-2.13.3")

# Create specific target used by us
add_library(github_catch2 INTERFACE)
target_include_directories(github_catch2
                           INTERFACE ${github_catch2_ROOT}/single_include)
