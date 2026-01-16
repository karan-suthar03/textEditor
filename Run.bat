@echo off
IF NOT EXIST out\TextEditor.exe (
    echo ❌ out\TextEditor.exe not found!
    echo Run build.bat first.
    pause
    exit /b
)

start out\TextEditor.exe