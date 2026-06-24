# Student Record Management System (C++)

A robust, object-oriented Student Record Management System developed in C++17. Designed as a demonstration of clean code, architectural patterns, and functional richness for a professional software engineering portfolio.

## 🚀 Key Features

- **Full CRUD Support**: Manage student profiles with persistent storage.
- **Academic Tracking**: Comprehensive course management with real-time GPA and academic standing calculation.
- **Data Persistence**: Robust file-based database using CSV format for transparency and interoperability.
- **Rich Analytics**:
  - GPA distribution and standard deviation.
  - Department-level performance metrics.
  - Academic probation tracking.
- **Advanced Searching & Sorting**: Multi-field search (ID, Name, Dept, Email) and flexible sorting (GPA, Name, ID).
- **Modern CLI Interface**: A polished, color-coded terminal UI with robust input validation.
- **Portability**: Statically linked build for cross-platform Windows deployment.

## 🛠️ Technology Stack

- **Language**: C++17
- **Compiler**: GCC (MinGW-w64 / MSYS2 UCRT64)
- **Concepts**: OOP, STL Containers, File I/O, Exception Handling, Lambda Expressions.
- **Build System**: Windows Batch Script (included).

## 📂 Project Structure

- `main.cpp`: CLI Menu system and feature orchestration.
- `Student.h/cpp`: The core entity model (Encapsulation).
- `StudentManager.h/cpp`: Business logic and collection management (Abstraction).
- `Utils.h/cpp`: Cross-cutting concerns like validation and console helpers.
- `Common.h`: Shared globally constant definitions (UI Colors, etc.).
- `build.bat`: Automated build pipeline.

## 🛠️ Build Instructions

### Prerequisites
- **MinGW-w64** (via MSYS2 recommended).
- Ensure `g++` is in your system path or edit `build.bat` to point to your compiler.

### Building
Run the included build script:
```powershell
./build.bat
```

### Running
Launch the compiled executable:
```powershell
./StudentRecordSystem.exe
```

## 📄 License

All rights reserved. This repository and its code are for personal portfolio review and viewing purposes only. No permission is granted to copy, modify, distribute, or use this code for any other purpose.
