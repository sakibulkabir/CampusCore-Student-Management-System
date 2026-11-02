#include "StudentManager.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

StudentManager::StudentManager(const std::string& storagePath) : storagePath_(storagePath) {}

bool StudentManager::load(std::string& error) {
    students_.clear();
    std::ifstream in(storagePath_);
    if (!in) {
        // A first run is valid: create the data directory/file on save.
        return true;
    }
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::vector<std::string> parts;
        std::string part;
        while (std::getline(ss, part, '|')) parts.push_back(part);
        if (parts.size() != 9) {
            error = "Invalid record at data file line " + std::to_string(lineNo) + ".";
            return false;
        }
        try {
            Student s;
            s.id = parts[0]; s.name = parts[1]; s.email = parts[2];
            s.phone = parts[3]; s.department = parts[4];
            s.semester = std::stoi(parts[5]); s.cgpa = std::stod(parts[6]);
            s.attendance = std::stod(parts[7]); s.status = parts[8];
            students_.push_back(s);
        } catch (...) {
            error = "Could not read record at data file line " + std::to_string(lineNo) + ".";
            return false;
        }
    }
    return true;
}

bool StudentManager::save(std::string& error) const {
    try {
        std::filesystem::path p(storagePath_);
        if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path());
        std::ofstream out(storagePath_, std::ios::trunc);
        if (!out) { error = "Could not write to " + storagePath_; return false; }
        out << std::fixed << std::setprecision(2);
        for (const auto& s : students_) {
            out << s.id << '|' << s.name << '|' << s.email << '|' << s.phone << '|'
                << s.department << '|' << s.semester << '|' << s.cgpa << '|'
                << s.attendance << '|' << s.status << '\n';
        }
        if (!out.good()) { error = "An error occurred while saving student data."; return false; }
        return true;
    } catch (const std::exception& e) {
        error = e.what(); return false;
    }
}

bool StudentManager::add(const Student& student, std::string& error) {
    if (findById(student.id)) { error = "A student with this ID already exists."; return false; }
    students_.push_back(student);
    if (!save(error)) { students_.pop_back(); return false; }
    return true;
}

bool StudentManager::update(const std::string& id, const Student& replacement, std::string& error) {
    for (auto& s : students_) {
        if (s.id == id) {
            Student old = s;
            s = replacement;
            s.id = id;
            if (!save(error)) { s = old; return false; }
            return true;
        }
    }
    error = "Student ID not found.";
    return false;
}

bool StudentManager::remove(const std::string& id, std::string& error) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [&](const Student& s) { return s.id == id; });
    if (it == students_.end()) { error = "Student ID not found."; return false; }
    Student old = *it;
    auto index = static_cast<std::size_t>(std::distance(students_.begin(), it));
    students_.erase(it);
    if (!save(error)) { students_.insert(students_.begin() + static_cast<long>(index), old); return false; }
    return true;
}

const std::vector<Student>& StudentManager::all() const { return students_; }

std::string StudentManager::lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::vector<Student> StudentManager::search(const std::string& query) const {
    std::vector<Student> result;
    const std::string q = lower(query);
    for (const auto& s : students_) {
        if (lower(s.id).find(q) != std::string::npos ||
            lower(s.name).find(q) != std::string::npos ||
            lower(s.email).find(q) != std::string::npos ||
            lower(s.department).find(q) != std::string::npos ||
            lower(s.phone).find(q) != std::string::npos) result.push_back(s);
    }
    return result;
}

std::vector<Student> StudentManager::sortedByName() const {
    auto copy = students_;
    std::sort(copy.begin(), copy.end(), [](const Student& a, const Student& b) {
        return lower(a.name) < lower(b.name);
    });
    return copy;
}

std::vector<Student> StudentManager::sortedByCgpa() const {
    auto copy = students_;
    std::sort(copy.begin(), copy.end(), [](const Student& a, const Student& b) {
        return a.cgpa > b.cgpa;
    });
    return copy;
}

const Student* StudentManager::findById(const std::string& id) const {
    for (const auto& s : students_) if (s.id == id) return &s;
    return nullptr;
}

std::string StudentManager::generateId() const {
    int highest = 1000;
    for (const auto& s : students_) {
        try {
            if (s.id.rfind("STU-", 0) == 0) highest = std::max(highest, std::stoi(s.id.substr(4)));
        } catch (...) {}
    }
    std::ostringstream out;
    out << "STU-" << std::setw(4) << std::setfill('0') << highest + 1;
    return out.str();
}

std::string StudentManager::csvEscape(const std::string& value) {
    std::string out = "\"";
    for (char c : value) { if (c == '"') out += '"'; out += c; }
    return out + "\"";
}

bool StudentManager::exportCsv(const std::string& path, std::string& error) const {
    std::ofstream out(path, std::ios::trunc);
    if (!out) { error = "Could not create export file."; return false; }
    out << "Student ID,Name,Email,Phone,Department,Semester,CGPA,Attendance,Status\n";
    out << std::fixed << std::setprecision(2);
    for (const auto& s : students_) {
        out << csvEscape(s.id) << ',' << csvEscape(s.name) << ',' << csvEscape(s.email) << ','
            << csvEscape(s.phone) << ',' << csvEscape(s.department) << ','
            << s.semester << ',' << s.cgpa << ',' << s.attendance << ',' << csvEscape(s.status) << '\n';
    }
    if (!out.good()) { error = "Failed while writing export file."; return false; }
    return true;
}

int StudentManager::total() const { return static_cast<int>(students_.size()); }
int StudentManager::activeCount() const {
    return static_cast<int>(std::count_if(students_.begin(), students_.end(),
        [](const Student& s) { return s.status == "Active"; }));
}
double StudentManager::averageCgpa() const {
    if (students_.empty()) return 0.0;
    double sum = 0; for (const auto& s : students_) sum += s.cgpa;
    return sum / students_.size();
}
double StudentManager::averageAttendance() const {
    if (students_.empty()) return 0.0;
    double sum = 0; for (const auto& s : students_) sum += s.attendance;
    return sum / students_.size();
}
