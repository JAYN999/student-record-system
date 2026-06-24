cmake_minimum_required(VERSION 3.15)
project(StudentRecordSystem LANGUAGES CXX)

# ── Require C++17 ────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Sources ──────────────────────────────────────────────────────────────────
set(SOURCES
    main.cpp
    Student.cpp
    StudentManager.cpp
    Utils.cpp
)

add_executable(StudentRecordSystem ${SOURCES})

# ── Install hint ─────────────────────────────────────────────────────────────
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
