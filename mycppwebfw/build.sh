#!/bin/bash
set -e

# Command line options
BUILD_TYPE="Release"
RUN_TESTS=false
RUN_BENCHMARKS=false
ENABLE_SANITIZERS=false
STATIC_ANALYSIS=false
CLEAN_BUILD=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug) BUILD_TYPE="Debug"; shift ;;
        --test) RUN_TESTS=true; shift ;;
        --benchmark) RUN_BENCHMARKS=true; shift ;;
        --sanitizers) ENABLE_SANITIZERS=true; shift ;;
        --analyze) STATIC_ANALYSIS=true; shift ;;
        --clean) CLEAN_BUILD=true; shift;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --debug         Build in Debug mode (default: Release)"
            echo "  --test          Run tests after building"
            echo "  --benchmark     Run benchmarks after building"
            echo "  --sanitizers    Enable sanitizers (ASan, TSan, MSan)"
            echo "  --analyze       Enable static analysis with Clang-Tidy"
            echo "  --clean         Clean build directory before building"
            echo "  -h, --help      Show this help message"
            exit 0
            ;;
        *) echo "Unknown option $1"; exit 1 ;;
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

# Build with appropriate flags
cmake -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DENABLE_TESTING=$RUN_TESTS \
    -DENABLE_BENCHMARKS=$RUN_BENCHMARKS \
    -DENABLE_SANITIZERS=$ENABLE_SANITIZERS \
    -DENABLE_STATIC_ANALYSIS=$STATIC_ANALYSIS

make -C build -j$(nproc)

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo "🧪 Running tests..."
    cd build && ctest --output-on-failure
    cd ..
fi

# Run benchmarks if requested
if [ "$RUN_BENCHMARKS" = true ]; then
    echo "🚀 Running benchmarks..."
    # This assumes you have a benchmark runner target named 'run_benchmarks'
    cd build && ./benchmarks/run_benchmarks
    cd ..
fi

echo "✅ Build completed successfully!"
echo "📦 Binaries are available in the build directory"

# Show available executables
if [ -d "build/bin" ]; then
    echo "🎯 Available executables:"
    find build/bin -type f -executable | sed 's/^/  /'
fi
