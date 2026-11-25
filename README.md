# Student Record Management System (C++)

A console-based student record management system written in **C++17**. It stores student records in a local text/CSV-style data file and provides student, course, search, reporting and export features.

## Features

- Add, update, search and list student records
- Soft-delete, restore and permanently delete records
- Add, update and remove courses for a student
- Calculate weighted GPA, average marks and academic standing
- Search by ID, name, department, email, phone and course code
- Sort by ID, name, GPA, department, semester and age
- Generate summary reports and export student data to CSV
- Validate common console inputs and handle malformed data-file rows

## Technology

- **Language:** C++17
- **Concepts:** Classes and objects, encapsulation, STL containers, algorithms, file I/O, exceptions and lambda functions
- **Persistence:** Local file-based records; CSV export
- **Build:** CMake, Makefile and a Windows batch build script
- **Compiler:** GCC/MinGW-w64 or another C++17-compatible compiler

## Build

### CMake (recommended)

```bash
cmake -S . -B build
cmake --build build
```

Run the executable produced in the `build` directory.

### Windows batch script

The included `build.bat` is configured for an MSYS2 UCRT64 installation at `C:\msys64\ucrt64\bin\g++.exe`. Update that path if your compiler is installed elsewhere, or use CMake instead.

## Data

The program reads and writes its local data file relative to the current working directory. Keep a backup of this file before testing destructive operations. The CSV export is intended for spreadsheet use and quotes text fields containing commas, quotes or line breaks.

## Project structure

- `main.cpp`: menu and application flow
- `Student.h/.cpp`: student and course data, grade calculations and record serialization
- `StudentManager.h/.cpp`: record operations, search, sorting, reports and persistence
- `Utils.h/.cpp`: console input and validation helpers
- `Common.h`: shared terminal color definitions
- `CMakeLists.txt`, `Makefile`, `build.bat`: build options

## Scope

This is a learning/portfolio console application, not a university information system or a production database service. Its local file storage is not a substitute for a relational database.

## License

All rights reserved. This repository and its code are for personal portfolio review and viewing purposes only.
