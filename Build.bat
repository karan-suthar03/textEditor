@echo off

if not exist out (
    mkdir out
)

g++ -g src\*.cpp -I include -o out\TextEditor.exe -lgdi32 -luser32

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Build failed!
    exit /b
)