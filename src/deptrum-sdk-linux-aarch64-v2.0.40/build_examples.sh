#!/bin/bash

# Copyright (c) Deptrum Inc. All Rights Reserved.

set -euo pipefail

# Function to check package installation
check_install_package() {
    local package=$1
    local description=$2
    
    if ! command -v "$package" &>/dev/null; then
        echo "$description could not be found. It is required to build the examples."
        read -rp "Do you want to install $package? (y/n) " answer
        if [[ "$answer" == [Yy]* ]]; then
            sudo apt-get install -y "$package"
        fi
    else
        echo "$package is installed."
    fi
}

# Check apt availability
echo "Checking if apt-get is workable..."
apt_workable=1
if ! command -v apt-get &>/dev/null; then
    echo "apt-get could not be found."
    apt_workable=0
elif ! sudo apt-get update &>/dev/null; then
    echo "apt-get update failed. apt-get may not be working properly."
    apt_workable=0
fi

# Install dependencies if apt is available
if (( apt_workable == 1 )); then
    # Check and install build tools
    if ! { g++ --version &>/dev/null && make --version &>/dev/null; }; then
        check_install_package "build-essential" "C++ Compiler and tools"
    fi
    
    # Check and install cmake
    check_install_package "cmake" "CMake"
    
    # Check OpenCV installation
    if ! { dpkg -l | grep -q libopencv-dev && dpkg -l | grep -q libopencv; }; then
        echo "OpenCV libraries could not be found. Without OpenCV, part of the examples may not build successfully."
        read -rp "Do you want to install OpenCV? (y/n) " answer
        if [[ "$answer" == [Yy]* ]]; then
            sudo apt-get install -y libopencv libopencv-dev
        fi
    else
        echo "OpenCV is installed."
    fi

    # Check and install GLFW
    check_install_package "libglfw3-dev" "GLFW development files"

    # Check and install OpenGL
    check_install_package "libglvnd-dev" "OpenGL development files"
    check_install_package "libgl1-mesa-dev" "OpenGL development files"
    check_install_package "libglu1-mesa-dev" "GLU development files"
else
    echo "apt-get is not workable. Build examples may not be successful."
fi

# Build process
current_dir=$(pwd)
trap 'cd "$current_dir"' EXIT

cd "$(dirname "$0")"
project_dir=$(pwd)
examples_dir="$project_dir/examples"

# Determine optimal thread count
cpu_count=$(nproc)
thread_count=$(( cpu_count > 1 ? cpu_count/2 : 1 ))

# CMake build
echo "Building examples..."
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DDT_BUILD_LINUX=ON \
      -DCMAKE_INSTALL_PREFIX="$project_dir" \
      "$examples_dir"

echo "Building examples with $thread_count threads..."
cmake --build . -- -j"$thread_count"
make install

# Clean up
cd "$project_dir" && rm -rf build

echo "Deptrum examples built successfully!"
echo "The executable files are located in: $project_dir/bin"