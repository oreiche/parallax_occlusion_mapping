#!/bin/sh

set -eu

readonly INSTALL_PREFIX=${1:-~/.local}
readonly CMAKE_INSTALLER="https://github.com/Kitware/CMake/releases/download/v3.19.0/cmake-3.19.0-Linux-x86_64.sh"

sudo apt update
sudo apt install -y --no-install-recommends \
    libopengl-dev libglx-dev libxinerama-dev libxrandr-dev libxcursor-dev \
    libxi-dev libgl1-mesa-dev

mkdir -p "$INSTALL_PREFIX"
cd "$INSTALL_PREFIX"
wget "$CMAKE_INSTALLER" -O cmake_installer.sh
chmod u+x cmake_installer.sh
./cmake_installer.sh --skip-license --prefix=.
