@echo off
cd /d "%~dp0"
gcc main.c dispatch.c heap.c queue.c hash.c graph.c render.c -o rescue.exe
if %errorlevel% neq 0 (
    echo.
    echo BUILD FAILED  -  check the gcc errors above
    exit /b %errorlevel%
)
"%~dp0rescue.exe"
