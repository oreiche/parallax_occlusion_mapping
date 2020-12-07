# Get SAIL from GitHub
http_archive(
    NAME    github_sail
    URL     "https://github.com/smoked-herring/sail/archive/v0.9.0-pre10.zip"
    SHA256  ca54b1db749ab7c0ce8241fd564450788ce8be7ab699405ce99a17c500720a96
    STRIP   "sail-0.9.0-pre10")


# Add targets from SAIL
add_subdirectory(${github_sail_ROOT}
                 ${github_sail_ROOT}/.build EXCLUDE_FROM_ALL)

# Collect and stage the SAIL's headers ourself
file(GLOB github_sail_HEADERS ${github_sail_ROOT}/src/libsail/*.h)
file(GLOB github_sail_CXX_HEADERS ${github_sail_ROOT}/src/bindings/c++/*.h)
file(GLOB github_sail_COMMON_HEADERS ${github_sail_ROOT}/src/libsail-common/*.h)
file(COPY ${github_sail_HEADERS} DESTINATION ${github_sail_ROOT}/include/sail/)
file(COPY ${github_sail_CXX_HEADERS} DESTINATION ${github_sail_ROOT}/include/sail-c++/)
file(COPY ${github_sail_COMMON_HEADERS} DESTINATION ${github_sail_ROOT}/include/sail-common/)
file(COPY ${github_sail_ROOT}/.build/include/config.h DESTINATION ${github_sail_ROOT}/include/sail-common/)

# Build and stage SAIL's codec shared libs to github_sail_CODECS_PATH
set(github_sail_CODECS_PATH ${github_sail_ROOT}/bin/codecs)
file(MAKE_DIRECTORY ${github_sail_CODECS_PATH})
add_custom_command(
    OUTPUT ${github_sail_CODECS_PATH}/sail-png.codec.info
           ${github_sail_CODECS_PATH}/sail-jpeg.codec.info
    DEPENDS sail-codec-png sail-codec-jpeg
    COMMAND ${CMAKE_COMMAND} -E copy
                    $<TARGET_FILE:sail-codec-png>
                    $<TARGET_FILE:sail-codec-jpeg>
                    ${github_sail_ROOT}/.build/src/sail-codecs/png/sail-png.codec.info
                    ${github_sail_ROOT}/.build/src/sail-codecs/jpeg/sail-jpeg.codec.info
                    ${github_sail_CODECS_PATH})
add_custom_target(sail_codecs DEPENDS
                  ${github_sail_CODECS_PATH}/sail-png.codec.info
                  ${github_sail_CODECS_PATH}/sail-jpeg.codec.info)

# Create specific target used by us
add_library(github_sail INTERFACE)
target_link_libraries(github_sail INTERFACE sail-c++)
target_include_directories(github_sail INTERFACE ${github_sail_ROOT}/include)
add_dependencies(github_sail sail_codecs)
add_compile_definitions(github_sail SAIL_MY_CODECS_PATH="${github_sail_CODECS_PATH}")