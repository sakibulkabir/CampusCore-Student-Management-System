#pragma once
#include "StudentManager.h"

class ConsoleUI {
public:
    explicit ConsoleUI(StudentManager& manager);
    void run();
private:
    StudentManager& manager_;
    void dashboard();
    void addStudent();
    void listStudents();
    void searchStudents();
    void updateStudent();
    void deleteStudent();
    void reports();
    void exportData();
    void showStudent(const Student& student) const;
};
