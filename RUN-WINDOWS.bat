@echo off
cd /d "%~dp0"
if not exist public\index.html (
 echo Frontend files are missing. Please extract the full ZIP again.
 pause
 exit /b 1
)
echo Building CampusCore Web...
set PATH=%PATH%;C:\msys64\ucrt64\bin
g++ -std=c++17 -Wall -Wextra -Iinclude src\server.cpp src\StudentManager.cpp -lws2_32 -o CampusCore-Web.exe
if errorlevel 1 (
 echo.
 echo Build failed. Check that MSYS2 UCRT64 is installed.
 pause
 exit /b 1
)
echo.
echo Starting server. Keep this window open.
start "CampusCore Browser" http://localhost:8080
CampusCore-Web.exe
pause
