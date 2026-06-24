@echo off
echo ============================================
echo   Student Record System - Build Script
echo ============================================
set GXX=C:\msys64\ucrt64\bin\g++.exe
set PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%
set CXXFLAGS=-std=c++17 -Wall -Wextra -O2 -static -static-libgcc -static-libstdc++
set TARGET=StudentRecordSystem.exe
set SRCS=main.cpp Student.cpp StudentManager.cpp Utils.cpp

echo.
echo [1/2] Compiling sources...
%GXX% %CXXFLAGS% -c main.cpp -o main.o
%GXX% %CXXFLAGS% -c Student.cpp -o Student.o
%GXX% %CXXFLAGS% -c StudentManager.cpp -o StudentManager.o
%GXX% %CXXFLAGS% -c Utils.cpp -o Utils.o

if %ERRORLEVEL% NEQ 0 (
    echo [FAIL] Compilation failed.
    pause
    exit /b 1
)

echo [2/2] Linking...
%GXX% %CXXFLAGS% -o %TARGET% main.o Student.o StudentManager.o Utils.o

if %ERRORLEVEL% NEQ 0 (
    echo [FAIL] Link failed.
    pause
    exit /b 1
)

echo.
echo ============================================
echo   BUILD SUCCESSFUL: %TARGET%
echo ============================================
del *.o 2>nul
