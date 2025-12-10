@echo off
REM Build script for Windows
REM Usage: scripts\build.bat [release|debug|test]

SET BUILD_TYPE=%1
IF "%BUILD_TYPE%"=="" SET BUILD_TYPE=release

echo ╔══════════════════════════════════════════════════════════════╗
echo ║          CPU Scheduler Simulator - Build Script             ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

IF "%BUILD_TYPE%"=="release" (
    echo Building release version...
    make clean
    make build
    echo ✓ Release build complete: bin\scheduler.exe
    GOTO :END
)

IF "%BUILD_TYPE%"=="debug" (
    echo Building debug version...
    make clean
    make debug
    echo ✓ Debug build complete: bin\scheduler_debug.exe
    GOTO :END
)

IF "%BUILD_TYPE%"=="test" (
    echo Building and running tests...
    make clean
    make test
    echo ✓ Tests complete
    GOTO :END
)

IF "%BUILD_TYPE%"=="all" (
    echo Building all targets...
    make clean
    make build
    make debug
    make test
    echo ✓ All builds complete
    GOTO :END
)

echo Error: Unknown build type '%BUILD_TYPE%'
echo Usage: %0 [release^|debug^|test^|all]
exit /b 1

:END
echo.
echo Build complete!
