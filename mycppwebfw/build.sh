#!/bin/bash

set -e

# Create a build directory if it doesn't exist
mkdir -p build

# Default cmake options
CMAKE_OPTIONS="-S ."

# Check for --tests flag
if [[ "$1" == "--tests" ]]; then
  CMAKE_OPTIONS="$CMAKE_OPTIONS -DENABLE_TESTING=ON"
fi

# Configure the project
cmake -B build $CMAKE_OPTIONS

# Build the project
cmake --build build

echo "✅ Build completed successfully!"
echo "📦 Binaries are available in the build directory"
echo "🎯 Available executables:"
find build/bin -type f -executable -printf "%f\n"
