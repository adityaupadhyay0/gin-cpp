#!/bin/bash

# Build script for mycppwebfw - C++ Web Framework
set -e  # Exit on any error

echo "🚀 Building mycppwebfw C++ Web Framework..."

# Parse command line arguments
BUILD_TYPE="Release"
CLEAN_BUILD=false
RUN_TESTS=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -t|--test)
            RUN_TESTS=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -d, --debug     Build in Debug mode (default: Release)"
            echo "  -c, --clean     Clean build directory before building"
            echo "  -t, --test      Run tests after building"
            echo "  -h, --help      Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ] && [ -d "build" ]; then
    echo "🧹 Cleaning build directory..."
    rm -rf build/*
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir build
fi

cd build

# Configure with CMake
echo "⚙️  Configuring CMake (Build Type: $BUILD_TYPE)..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build the project
echo "🔨 Building project..."
make -j$(nproc)

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo "🧪 Running tests..."
    if [ -f "tests/test_runner" ] || command -v ctest &> /dev/null; then
        ctest --output-on-failure
    else
        echo "⚠️  No tests found or ctest not available"
    fi
fi

echo "✅ Build completed successfully!"
echo "📦 Binaries are available in the build directory"

# Show available executables
if [ -d "bin" ]; then
    echo "🎯 Available executables:"
    find bin -type f -executable | sed 's/^/  /'
fi
