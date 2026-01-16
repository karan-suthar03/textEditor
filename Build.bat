@echo off

if not exist out (
    mkdir out
)

g++ src\*.cpp -I include -mwindows -o out\TextEditor.exe

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Build failed!
    exit /b
)