set(POM_EXTERNAL_DIR "${CMAKE_BINARY_DIR}/external")

if (NOT EXISTS "${POM_TOOLCHAIN_DIR}")
    # get toolchain from the internet
    file(DOWNLOAD "https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz"
         "${CMAKE_BINARY_DIR}/llvm-toolchain.zip"
         TIMEOUT 60
         EXPECTED_HASH SHA256=829f5fb0ebda1d8716464394f97d5475d465ddc7bea2879c0601316b611ff6db)
    file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/llvm-toolchain.zip"
         DESTINATION "${POM_EXTERNAL_DIR}/toolchain")
    set(POM_TOOLCHAIN_DIR "${POM_EXTERNAL_DIR}/toolchain/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
                    "${POM_TOOLCHAIN_DIR}"
                    "${CMAKE_SOURCE_DIR}/.toolchain")
    set(CMAKE_C_COMPILER "${POM_TOOLCHAIN_DIR}/bin/clang")
    set(CMAKE_CXX_COMPILER "${POM_TOOLCHAIN_DIR}/bin/clang++")
endif()

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wsign-conversion -Werror -pedantic")

add_subdirectory(src)