#!/bin/bash

set -e 

# Variables
BUILD_TYPE=Release # Debug, Release, RelWithDebInfo, MinSizeRel
BUILD_DIR=build # Build Directory

if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating Build Directory: $BUILD_DIR"
  mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo "Running Cmake Configrations...."
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

echo "Building project.."
cmake --build . --config $BUILD_TYPE -- -j$(nproc)

echo "Build finished successfully."
