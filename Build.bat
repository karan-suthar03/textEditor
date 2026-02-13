@echo off

if not exist out (
    mkdir out
)

g++ -std=c++17 -g src\*.cpp -I include -o out\TextEditor.exe -lgdi32 -luser32 -DNDEBUG
