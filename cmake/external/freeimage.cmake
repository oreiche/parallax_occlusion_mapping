include(FetchContent)

# Get in-official FreeImage from GitHub
FetchContent_Declare(
    github_freeimage
    GIT_REPOSITORY https://github.com/oreiche/FreeImage-Cmake.git
    GIT_TAG        master)

# Add targets from FreeImage
set(SUPPORT_FMT_JPEG ON CACHE BOOL "Enable JPEG")
FetchContent_MakeAvailable(github_freeimage)

# Create specific target used by us
add_library(github_freeimage INTERFACE)
target_include_directories(github_freeimage
                           INTERFACE ${github_freeimage_SOURCE_DIR}/Source)
target_link_libraries(github_freeimage INTERFACE FreeImage)

# C++ wrapper library FreeImagePlus
file(GLOB FREEIMAGEPLUS_SRCS
     ${github_freeimage_SOURCE_DIR}/Wrapper/FreeImagePlus/src/*.cpp)
add_library(github_freeimageplus STATIC ${FREEIMAGEPLUS_SRCS})
target_include_directories(github_freeimageplus
    PUBLIC ${github_freeimage_SOURCE_DIR}/Wrapper/FreeImagePlus)
target_link_libraries(github_freeimageplus PUBLIC github_freeimage)