# Get in-official FreeImage from GitHub
http_archive(
    NAME    github_freeimage
    URL     "https://github.com/oreiche/FreeImage-Cmake/archive/44c0bee6f05bb2d98b762ca7f8e25a773da42644.zip"
    SHA256  c2cded8dc1143e4656cf018def68ce7864c98ce63c3223cc738ec5dd55bbc75f
    STRIP   "FreeImage-Cmake-44c0bee6f05bb2d98b762ca7f8e25a773da42644")


# Add targets from FreeImage
set(SUPPORT_FMT_JPEG ON CACHE BOOL "Enable JPEG")
add_subdirectory(${github_freeimage_ROOT})

# Create specific target used by us
add_library(github_freeimage INTERFACE)
target_include_directories(github_freeimage
                           INTERFACE ${github_freeimage_ROOT}/Source)
target_link_libraries(github_freeimage INTERFACE FreeImage)

# C++ wrapper library FreeImagePlus
file(GLOB FREEIMAGEPLUS_SRCS
     ${github_freeimage_ROOT}/Wrapper/FreeImagePlus/src/*.cpp)
add_library(github_freeimageplus STATIC ${FREEIMAGEPLUS_SRCS})
target_include_directories(github_freeimageplus
    PUBLIC ${github_freeimage_ROOT}/Wrapper/FreeImagePlus)
target_link_libraries(github_freeimageplus PUBLIC github_freeimage)