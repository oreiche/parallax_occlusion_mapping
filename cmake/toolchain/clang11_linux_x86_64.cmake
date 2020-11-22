list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(externals)

# Get Clang 11 from GitHub
http_archive(
    NAME    llvm_toolchain
    URL     "https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz"
    SHA256  829f5fb0ebda1d8716464394f97d5475d465ddc7bea2879c0601316b611ff6db
    STRIP   "clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04")

# Set compiler paths
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER "${llvm_toolchain_ROOT}/bin/clang" CACHE FILEPATH "C Compiler")
set(CMAKE_CXX_COMPILER "${llvm_toolchain_ROOT}/bin/clang++" CACHE FILEPATH "C++ Compiler")
