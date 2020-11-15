#!/bin/sh

set -eu

readonly INSTALL_DIR=${1:-/tmp/cmake}
readonly DOWNLOAD_URL="https://github.com/Kitware/CMake/releases/download/v3.19.0/cmake-3.19.0-Linux-x86_64.sh"

mkdir -p "$INSTALL_DIR"
cd "$INSTALL_DIR"
wget "$DOWNLOAD_URL" -O cmake_installer.sh
chmod u+x cmake_installer.sh
./cmake_installer.sh --skip-license --prefix=.
