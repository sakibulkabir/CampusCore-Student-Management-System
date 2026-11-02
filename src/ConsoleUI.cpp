#include "ConsoleUI.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
const char* RESET = "\033[0m";
const char* PURPLE = "\033[38;5;141m";
const char* CYAN = "\033[38;5;80m";
const char* GREEN = "\033[38;5;114m";
const char* RED = "\033[38;5;203m";
const char* MUTED = "\033[38;5;245m";
const char* BOLD = "\033[1m";

void clearScreen() { std::cout << "\033[2J\033[H"; }
void pauseScreen() { std::cout << "\n" << MUTED << "Press ENTER to continue..." << RESET; std::string x; std::getline(std::cin, x); }
void line() { std::cout << MUTED << "  ------------------------------------------------------------------------------" << RESET << "\n"; }
std::string trim(const std::string& s) {
    auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}
std::string readText(const std::string& label, const std::string& current = "") {
    std::cout << "  " << CYAN << label << RESET;
    if (!current.empty()) std::cout << " " << MUTED << "[" << current << "]" << RESET;
    std::cout << ": ";
    std::string value; std::getline(std::cin, value);
    value = trim(value);
    return value.empty() ? current : value;
}
int readInt(const std::string& label, int current, int min, int max) {
    while (true) {
        std::string s = readText(label, std::to_string(current));
        try { std::size_t n; int v = std::stoi(s, &n); if (n == s.size() && v >= min && v <= max) return v; } catch (...) {}
        std::cout << "  " << RED << "Enter a number from " << min << " to " << max << "." << RESET << "\n";
    }
}
double readDouble(const std::string& label, double current, double min, double max) {
    while (true) {
        std::ostringstream d; d << std::fixed << std::setprecision(2) << current;
        std::string s = readText(label, d.str());
        try { std::size_t n; double v = std::stod(s, &n); if (n == s.size() && v >= min && v <= max) return v; } catch (...) {}
        std::cout << "  " << RED << "Enter a value from " << min << " to " << max << "." << RESET << "\n";
    }
}
bool validField(const std::string& s) { return s.find('|') == std::string::npos && s.find('\n') == std::string::npos && s.find('\r') == std::string::npos; }
void printTable(const std::vector<Student>& students) {
    if (students.empty()) { std::cout << "  " << MUTED << "No student records found." << RESET << "\n"; return; }
    std::cout << "  " << BOLD << std::left << std::setw(12) << "ID" << std::setw(23) << "NAME"
              << std::setw(18) << "DEPARTMENT" << std::setw(8) << "SEM" << std::setw(8)
              << "CGPA" << std::setw(10) << "ATTEND." << "STATUS" << RESET << "\n";
    line();
    for (const auto& s : students) {
        std::cout << "  " << std::left << std::setw(12) << s.id
                  << std::setw(23) << (s.name.size() > 21 ? s.name.substr(0, 20) + "." : s.name)
                  << std::setw(18) << (s.department.size() > 16 ? s.department.substr(0, 15) + "." : s.department)
                  << std::setw(8) << s.semester << std::setw(8) << std::fixed << std::setprecision(2) << s.cgpa
                  << std::setw(10) << (std::to_string(static_cast<int>(s.attendance)) + "%")
                  << (s.status == "Active" ? GREEN : MUTED) << s.status << RESET << "\n";
    }
}
}

ConsoleUI::ConsoleUI(StudentManager& manager) : manager_(manager) {}

void ConsoleUI::run() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode)) SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    while (true) {
        clearScreen();
        dashboard();
        std::cout << "\n  " << PURPLE << BOLD << "[1]" << RESET << " Add student"
                  << "      " << PURPLE << BOLD << "[2]" << RESET << " View students"
                  << "      " << PURPLE << BOLD << "[3]" << RESET << " Search\n";
        std::cout << "  " << PURPLE << BOLD << "[4]" << RESET << " Update student"
                  << "   " << PURPLE << BOLD << "[5]" << RESET << " Delete student"
                  << "     " << PURPLE << BOLD << "[6]" << RESET << " Reports & ranking\n";
        std::cout << "  " << PURPLE << BOLD << "[7]" << RESET << " Export CSV"
                  << "       " << PURPLE << BOLD << "[0]" << RESET << " Exit\n";
        line();
        std::cout << "  " << CYAN << "Select an option" << RESET << ": ";
        std::string choice; std::getline(std::cin, choice);
        if (choice == "1") addStudent();
        else if (choice == "2") listStudents();
        else if (choice == "3") searchStudents();
        else if (choice == "4") updateStudent();
        else if (choice == "5") deleteStudent();
        else if (choice == "6") reports();
        else if (choice == "7") exportData();
        else if (choice == "0") { clearScreen(); std::cout << GREEN << "\n  Thanks for using CampusCore. See you!\n\n" << RESET; return; }
        else { std::cout << RED << "\n  Invalid option." << RESET; pauseScreen(); }
    }
}

void ConsoleUI::dashboard() {
    std::cout << PURPLE << BOLD;
    std::cout << "   _____                                  _____ore\n";
    std::cout << "  / ____|                                / ____|\n";
    std::cout << " | |     __ _ _ __ ___  _ __  _   _ ___ | |     ___  _ __ ___\n";
    std::cout << " | |    / _` | '_ ` _ \\| '_ \\| | | / __|| |    / _ \\| '__/ _ \\\n";
    std::cout << " | |___| (_| | | | | | | |_) | |_| \\__ \\| |___| (_) | | |  __/\n";
    std::cout << "  \\_____\\__,_|_| |_| |_| .__/ \\__,_|___/ \\_____\\___/|_|  \\___|\n";
    std::cout << "                       | |\n                       |_|\n" << RESET;
    std::cout << "  " << MUTED << "STUDENT MANAGEMENT SUITE  /  C++17 EDITION" << RESET << "\n";
    line();
    std::cout << "  " << BOLD << "OVERVIEW" << RESET << "       "
              << CYAN << "TOTAL STUDENTS " << RESET << BOLD << manager_.total() << RESET
              << "       " << GREEN << "ACTIVE " << RESET << BOLD << manager_.activeCount() << RESET
              << "       " << PURPLE << "AVG CGPA " << RESET << BOLD << std::fixed << std::setprecision(2) << manager_.averageCgpa() << RESET
              << "       " << CYAN << "AVG ATTENDANCE " << RESET << BOLD << std::setprecision(1) << manager_.averageAttendance() << "%" << RESET << "\n";
    line();
}

void ConsoleUI::addStudent() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  ADD NEW STUDENT\n" << RESET; line();
    Student s; s.id = manager_.generateId();
    std::cout << "  Generated ID: " << GREEN << BOLD << s.id << RESET << "\n";
    do { s.name = readText("Full name"); if (s.name.empty() || !validField(s.name)) std::cout << RED << "  Name is required; | is not allowed.\n" << RESET; } while (s.name.empty() || !validField(s.name));
    do { s.email = readText("Email"); if (s.email.empty() || !validField(s.email)) std::cout << RED << "  Email is required; | is not allowed.\n" << RESET; } while (s.email.empty() || !validField(s.email));
    s.phone = readText("Phone");
    s.department = readText("Department");
    s.semester = readInt("Semester", 1, 1, 16);
    s.cgpa = readDouble("CGPA", 0.0, 0.0, 4.0);
    s.attendance = readDouble("Attendance (%)", 0.0, 0.0, 100.0);
    std::string status = readText("Status (Active / Inactive)", "Active");
    s.status = (status == "Inactive" || status == "inactive" || status == "INACTIVE") ? "Inactive" : "Active";
    std::string error;
    if (manager_.add(s, error)) std::cout << "\n  " << GREEN << "Student saved successfully." << RESET << "\n";
    else std::cout << "\n  " << RED << error << RESET << "\n";
    pauseScreen();
}

void ConsoleUI::listStudents() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  STUDENT DIRECTORY\n" << RESET; line();
    printTable(manager_.all()); pauseScreen();
}

void ConsoleUI::searchStudents() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  SEARCH STUDENTS\n" << RESET; line();
    std::string q = readText("Search by ID, name, email, phone or department");
    auto results = manager_.search(q);
    std::cout << "\n  " << CYAN << results.size() << " result(s)" << RESET << "\n";
    printTable(results); pauseScreen();
}

void ConsoleUI::showStudent(const Student& s) const {
    std::cout << "\n  " << BOLD << "STUDENT PROFILE" << RESET << "\n"; line();
    std::cout << "  ID          : " << s.id << "\n  Name        : " << s.name
              << "\n  Email       : " << s.email << "\n  Phone       : " << s.phone
              << "\n  Department  : " << s.department << "\n  Semester    : " << s.semester
              << "\n  CGPA        : " << std::fixed << std::setprecision(2) << s.cgpa
              << "\n  Attendance  : " << s.attendance << "%\n  Status      : " << s.status << "\n";
}

void ConsoleUI::updateStudent() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  UPDATE STUDENT\n" << RESET; line();
    std::string id = readText("Enter student ID");
    const Student* found = manager_.findById(id);
    if (!found) { std::cout << RED << "\n  Student not found." << RESET; pauseScreen(); return; }
    Student s = *found; showStudent(s);
    std::cout << "\n  " << MUTED << "Press ENTER to keep the current value." << RESET << "\n";
    s.name = readText("Full name", s.name); s.email = readText("Email", s.email);
    s.phone = readText("Phone", s.phone); s.department = readText("Department", s.department);
    s.semester = readInt("Semester", s.semester, 1, 16);
    s.cgpa = readDouble("CGPA", s.cgpa, 0.0, 4.0);
    s.attendance = readDouble("Attendance (%)", s.attendance, 0.0, 100.0);
    std::string status = readText("Status (Active / Inactive)", s.status);
    s.status = (status == "Inactive" || status == "inactive" || status == "INACTIVE") ? "Inactive" : "Active";
    std::string error;
    if (manager_.update(id, s, error)) std::cout << GREEN << "\n  Student updated." << RESET;
    else std::cout << RED << "\n  " << error << RESET;
    pauseScreen();
}

void ConsoleUI::deleteStudent() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  DELETE STUDENT\n" << RESET; line();
    std::string id = readText("Enter student ID");
    const Student* s = manager_.findById(id);
    if (!s) { std::cout << RED << "\n  Student not found." << RESET; pauseScreen(); return; }
    showStudent(*s);
    std::string confirm = readText("Type DELETE to permanently remove this record");
    if (confirm != "DELETE") { std::cout << MUTED << "\n  Delete cancelled." << RESET; pauseScreen(); return; }
    std::string error;
    if (manager_.remove(id, error)) std::cout << GREEN << "\n  Student deleted." << RESET;
    else std::cout << RED << "\n  " << error << RESET;
    pauseScreen();
}

void ConsoleUI::reports() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  REPORTS & ACADEMIC RANKING\n" << RESET; line();
    std::cout << "  Total records      : " << manager_.total()
              << "\n  Active students    : " << manager_.activeCount()
              << "\n  Inactive students  : " << manager_.total() - manager_.activeCount()
              << "\n  Average CGPA       : " << std::fixed << std::setprecision(2) << manager_.averageCgpa()
              << "\n  Average attendance : " << std::setprecision(1) << manager_.averageAttendance() << "%\n";
    line(); std::cout << "\n  " << BOLD << "TOP STUDENTS BY CGPA" << RESET << "\n";
    auto ranked = manager_.sortedByCgpa();
    if (ranked.size() > 5) ranked.resize(5);
    printTable(ranked);
    std::cout << "\n  " << BOLD << "STUDENTS BY NAME" << RESET << "\n";
    printTable(manager_.sortedByName());
    pauseScreen();
}

void ConsoleUI::exportData() {
    clearScreen(); std::cout << PURPLE << BOLD << "\n  EXPORT DATA\n" << RESET; line();
    std::string path = readText("CSV file path", "data/students_export.csv");
    std::string error;
    if (manager_.exportCsv(path, error)) std::cout << GREEN << "\n  Export completed: " << path << RESET << "\n";
    else std::cout << RED << "\n  " << error << RESET << "\n";
    pauseScreen();
}
