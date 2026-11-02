# CampusCore — Student Management System

A multi-file C++17 console application for managing student records. It uses a clean terminal dashboard, object-oriented design, and local file persistence.

## Features

- Add, view, search, update, and delete student records
- Automatically generated IDs (`STU-1001`, `STU-1002`, ...)
- Student profile details: name, email, phone, department, semester, CGPA, attendance, and status
- Input validation for semester, CGPA, and attendance
- Search by ID, name, email, phone, or department
- Dashboard statistics: total, active count, average CGPA, and average attendance
- Academic ranking by CGPA and alphabetical sorting
- Export all records to CSV for Excel / Google Sheets
- Automatic saving to `data/students.db`

## Project structure

```text
Student-Management-System/
├── data/
│   └── students.db
├── include/
│   ├── ConsoleUI.h
│   ├── Student.h
│   └── StudentManager.h
├── src/
│   ├── ConsoleUI.cpp
│   ├── main.cpp
│   └── StudentManager.cpp
├── .gitignore
├── CMakeLists.txt
├── Makefile
└── README.md
```

## Requirements

- A C++17 compiler: MinGW-w64 g++, GCC, or Clang
- Windows: PowerShell / Windows Terminal / VS Code terminal
- macOS or Linux: Terminal

## Run on Windows (recommended: VS Code + MinGW g++)

1. Install **VS Code** and a C++ compiler such as **MinGW-w64**.
2. Open this project folder in VS Code.
3. Open Terminal → New Terminal.
4. Check the compiler:

   ```powershell
   g++ --version
   ```

5. Compile the project:

   ```powershell
   g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/StudentManager.cpp src/ConsoleUI.cpp -o CampusCore.exe
   ```

6. Run it:

   ```powershell
   .\CampusCore.exe
   ```

If PowerShell says `g++` is not recognized, install MinGW-w64 and add its `bin` folder to your Windows PATH, then restart VS Code.

## Run on macOS / Linux

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/StudentManager.cpp src/ConsoleUI.cpp -o CampusCore
./CampusCore
```

## Run with CMake

```bash
cmake -S . -B build
cmake --build build
```

Run the executable from the generated build folder (the exact path varies by OS).

## How to use

- Select a menu number and press Enter.
- Add Student creates a new ID automatically.
- In Update Student, press Enter on a field to keep its current value.
- Delete requires typing the exact word `DELETE`.
- Export CSV writes a spreadsheet-friendly file. The default path is `data/students_export.csv`.
- Records are saved automatically after each successful add, update, or delete.

## Data storage note

The application stores records in a simple local text database at `data/students.db`. Keep this file when moving or backing up your project if you want to preserve your records. Do not edit it manually unless you understand the `|`-separated format.

## GitHub upload

Create a new **Public** repository named `Student-Management-System`, then run these commands from the project folder:

```bash
git init
git add .
git commit -m "Add CampusCore student management system"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/Student-Management-System.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username.

## Future improvements

Possible next steps: course-wise enrollment, attendance entry by date, fee management, login roles, and a graphical interface.
