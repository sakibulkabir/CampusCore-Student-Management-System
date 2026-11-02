# CampusCore Web — C++ Student Management System

This version connects the existing C++ StudentManager backend to a responsive browser dashboard. The backend uses the Windows Winsock API and serves the frontend itself; no Node.js or separate web server is required.

## Run on Windows
1. Extract the complete ZIP.
2. Make sure MSYS2 UCRT64 g++ is installed at `C:\msys64\ucrt64\bin`.
3. Double-click `RUN-WINDOWS.bat`.
4. Your browser opens `http://localhost:8080`. Keep the black server window open while using the dashboard.

If the browser does not open, manually visit `http://localhost:8080`. To stop the server, focus the black window and press Ctrl+C.

## Features
- C++ backend and persistent local data file (`data/students.db`)
- Browser dashboard with stats, student directory, search and status filter
- Add, edit and delete student records
- CSV export
- Responsive layout for desktop, tablet and mobile
- Validates semester (1–12), CGPA (0–4), and attendance (0–100)

## Build manually (CMD)
```bat
set PATH=%PATH%;C:\msys64\ucrt64\bin
g++ -std=c++17 -Wall -Wextra -Iinclude src\server.cpp src\StudentManager.cpp -lws2_32 -o CampusCore-Web.exe
CampusCore-Web.exe
```
Then open `http://localhost:8080`.

## GitHub
Upload the whole folder, including `public`, `src`, `include`, `data`, and `RUN-WINDOWS.bat`. This is a local application: GitHub hosts the source code, but it does not run the C++ server as a live website.
