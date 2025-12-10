#!/bin/bash
# Build script for CPU Scheduler Simulator
# Usage: ./scripts/build.sh [release|debug|test]

set -e  # Exit on error

BUILD_TYPE="${1:-release}"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "╔══════════════════════════════════════════════════════════════╗"
echo "║          CPU Scheduler Simulator - Build Script             ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

cd "$PROJECT_ROOT"

case "$BUILD_TYPE" in
    release)
        echo "Building release version..."
        make clean
        make build
        echo "✓ Release build complete: bin/scheduler"
        ;;
    
    debug)
        echo "Building debug version..."
        make clean
        make debug
        echo "✓ Debug build complete: bin/scheduler_debug"
        ;;
    
    test)
        echo "Building and running tests..."
        make clean
        make test
        echo "✓ Tests complete"
        ;;
    
    all)
        echo "Building all targets..."
        make clean
        make build
        make debug
        make test
        echo "✓ All builds complete"
        ;;
    
    *)
        echo "Error: Unknown build type '$BUILD_TYPE'"
        echo "Usage: $0 [release|debug|test|all]"
        exit 1
        ;;
esac

echo ""
echo "Build complete!"
