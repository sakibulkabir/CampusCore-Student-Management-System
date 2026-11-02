#pragma once
#include <string>

struct Student {
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    std::string department;
    int semester = 1;
    double cgpa = 0.0;
    double attendance = 0.0;
    std::string status = "Active";
};
