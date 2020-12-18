cmake_minimum_required(VERSION 3.14)

macro(GetToolchainBaseDir RESULT_VAR)
  # Default: ${CMAKE_BINARY_DIR}/_deps
  set(TOOLCHAIN_ROOT_PATH "${CMAKE_BINARY_DIR}")
  set(TOOLCHAIN_STAGE_DIR "_deps")
  # Prevent fetch for try_compile() by reusing existing toolchain base directory
  if ("${CMAKE_PROJECT_NAME}" STREQUAL "CMAKE_TRY_COMPILE")
      # Search for existing toolchain base directory in parent directories
      while (NOT EXISTS "${TOOLCHAIN_ROOT_PATH}/${TOOLCHAIN_STAGE_DIR}")
          get_filename_component(TOOLCHAIN_PARENT_DIR
                                 "${TOOLCHAIN_ROOT_PATH}" DIRECTORY)
          if ("${TOOLCHAIN_PARENT_DIR}" STREQUAL "${TOOLCHAIN_ROOT_PATH}")
              message(FATAL_ERROR "Could not find toolchain base directory.")
          endif()
          set(TOOLCHAIN_ROOT_PATH "${TOOLCHAIN_PARENT_DIR}")
      endwhile()
  endif()
  set(${RESULT_VAR} ${TOOLCHAIN_ROOT_PATH}/${TOOLCHAIN_STAGE_DIR})
endmacro()

include(FetchContent)
GetToolchainBaseDir(TOOLCHAIN_BASE_DIR)
set(FETCHCONTENT_BASE_DIR ${TOOLCHAIN_BASE_DIR})

# Get Clang 11 from GitHub
FetchContent_Declare(
    llvm_toolchain
    URL      "https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz"
    URL_HASH SHA256=829f5fb0ebda1d8716464394f97d5475d465ddc7bea2879c0601316b611ff6db)
FetchContent_MakeAvailable(llvm_toolchain)

# Set compiler paths
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER "${llvm_toolchain_SOURCE_DIR}/bin/clang" CACHE FILEPATH "C Compiler")
set(CMAKE_CXX_COMPILER "${llvm_toolchain_SOURCE_DIR}/bin/clang++" CACHE FILEPATH "C++ Compiler")

unset(FETCHCONTENT_BASE_DIR)