#pragma once
#include "Student.h"
#include <string>
#include <vector>

class StudentManager {
public:
    explicit StudentManager(const std::string& storagePath);
    bool load(std::string& error);
    bool save(std::string& error) const;
    bool add(const Student& student, std::string& error);
    bool update(const std::string& id, const Student& replacement, std::string& error);
    bool remove(const std::string& id, std::string& error);
    const std::vector<Student>& all() const;
    std::vector<Student> search(const std::string& query) const;
    std::vector<Student> sortedByName() const;
    std::vector<Student> sortedByCgpa() const;
    const Student* findById(const std::string& id) const;
    std::string generateId() const;
    bool exportCsv(const std::string& path, std::string& error) const;
    int total() const;
    int activeCount() const;
    double averageCgpa() const;
    double averageAttendance() const;
private:
    std::string storagePath_;
    std::vector<Student> students_;
    static std::string lower(std::string value);
    static std::string csvEscape(const std::string& value);
};
