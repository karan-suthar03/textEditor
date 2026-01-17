@echo off

if not exist out (
    mkdir out
)

g++ -std=c++17 -g src\*.cpp -I include -o out\TextEditor.exe -lgdi32 -luser32 -DNDEBUG

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Build failed!
    exit /b
)