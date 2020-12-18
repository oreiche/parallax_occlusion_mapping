#!/bin/sh

set -eu

sudo apt update
sudo apt install -y --no-install-recommends \
    libopengl-dev libglx-dev libxinerama-dev libxrandr-dev libxcursor-dev \
    libxi-dev libgl1-mesa-dev valgrind
