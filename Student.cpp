#include "Student.h"
#include "Common.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <iomanip>

Student::Student()
    : m_id(0), m_age(0), m_semester(1), m_active(true) {}

Student::Student(int id, const std::string& name, int age,
                 const std::string& gender, const std::string& email,
                 const std::string& phone, const std::string& department,
                 int semester, const std::string& enrollDate)
    : m_id(id), m_name(name), m_age(age), m_gender(gender),
      m_email(email), m_phone(phone), m_department(department),
      m_semester(semester), m_enrollDate(enrollDate), m_active(true) {}

void Student::addCourse(const Course& c) {
    if (c.marks < 0.0 || c.marks > 100.0)
        throw std::out_of_range("Course marks must be between 0 and 100.");
    if (c.credits < 1 || c.credits > 6)
        throw std::out_of_range("Course credits must be between 1 and 6.");
    if (hasCourse(c.code))
        throw std::runtime_error("Course '" + c.code + "' already enrolled.");
    m_courses.push_back(c);
}

bool Student::updateCourse(const std::string& code, double newMarks) {
    for (auto& c : m_courses) {
        if (c.code == code) {
            if (newMarks < 0 || newMarks > 100)
                throw std::out_of_range("Marks must be between 0 and 100.");
            c.marks = newMarks;
            return true;
        }
    }
    return false;
}

bool Student::removeCourse(const std::string& code) {
    auto it = std::remove_if(m_courses.begin(), m_courses.end(),
                             [&](const Course& c){ return c.code == code; });
    if (it == m_courses.end()) return false;
    m_courses.erase(it, m_courses.end());
    return true;
}

bool Student::hasCourse(const std::string& code) const {
    return std::any_of(m_courses.begin(), m_courses.end(),
                       [&](const Course& c){ return c.code == code; });
}

double Student::calculateGPA() const {
    if (m_courses.empty()) return 0.0;
    double totalPoints = 0.0;
    int    totalCr     = 0;
    for (const auto& c : m_courses) {
        totalPoints += c.gradePoint() * c.credits;
        totalCr     += c.credits;
    }
    return (totalCr == 0) ? 0.0 : totalPoints / totalCr;
}

double Student::calculateAverage() const {
    if (m_courses.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& c : m_courses) sum += c.marks;
    return sum / m_courses.size();
}

int Student::totalCredits() const {
    int t = 0;
    for (const auto& c : m_courses) t += c.credits;
    return t;
}

std::string Student::academicStanding() const {
    double gpa = calculateGPA();
    if (gpa >= 3.7) return "Dean's List";
    if (gpa >= 3.0) return "Good Standing";
    if (gpa >= 2.0) return "Satisfactory";
    if (gpa >= 1.0) return "Academic Probation";
    return "Academic Suspension";
}

void Student::printSummary() const {
    std::string status = m_active ? Color::GREEN + "Active" + Color::RESET
                                  : Color::RED   + "Inactive" + Color::RESET;
    std::cout << Color::BOLD << std::left
              << std::setw(6)  << m_id
              << std::setw(24) << m_name
              << std::setw(18) << m_department
              << std::setw(5)  << m_semester
              << std::setw(10) << std::fixed << std::setprecision(2) << calculateGPA()
              << Color::RESET  << status << "\n";
}

void Student::printDetailed() const {
    std::cout << "\n" << Color::CYAN << Color::BOLD
              << "  ╔══ STUDENT PROFILE ══════════════════════════════════╗\n"
              << Color::RESET;
    auto row = [](const std::string& label, const std::string& val){
        std::cout << Color::CYAN << "  ║  " << Color::RESET
                  << Color::BOLD << std::left << std::setw(15) << label
                  << Color::RESET << ": " << val << "\n";
    };
    row("ID",           std::to_string(m_id));
    row("Name",         m_name);
    row("Age",          std::to_string(m_age));
    row("Gender",       m_gender);
    row("Email",        m_email);
    row("Phone",        m_phone);
    row("Department",   m_department);
    row("Semester",     std::to_string(m_semester));
    row("Enrolled",     m_enrollDate);
    row("Status",       m_active ? "Active" : "Inactive");
    row("GPA",          std::to_string(calculateGPA()).substr(0,4));
    row("Standing",     academicStanding());
    std::cout << Color::CYAN << Color::BOLD
              << "  ╚═════════════════════════════════════════════════════╝\n"
              << Color::RESET;
}

void Student::printTranscript() const {
    printDetailed();
    if (m_courses.empty()) {
        std::cout << Color::YELLOW << "  No courses enrolled.\n" << Color::RESET;
        return;
    }
    std::cout << "\n" << Color::BOLD << Color::WHITE
              << "  " << std::left
              << std::setw(8)  << "Code"
              << std::setw(30) << "Course Name"
              << std::setw(8)  << "Credits"
              << std::setw(8)  << "Marks"
              << std::setw(6)  << "Grade"
              << std::setw(6)  << "GP"
              << Color::RESET  << "\n"
              << "  " << std::string(64, '-') << "\n";

    for (const auto& c : m_courses) {
        std::string gradeColor = (c.marks >= 70) ? Color::GREEN :
                                 (c.marks >= 50) ? Color::YELLOW : Color::RED;
        std::cout << "  " << std::left
                  << std::setw(8)  << c.code
                  << std::setw(30) << c.name
                  << std::setw(8)  << c.credits
                  << std::setw(8)  << std::fixed << std::setprecision(1) << c.marks
                  << gradeColor
                  << std::setw(6)  << c.letterGrade()
                  << std::setw(6)  << std::setprecision(1) << c.gradePoint()
                  << Color::RESET  << "\n";
    }
    std::cout << "  " << std::string(64, '-') << "\n"
              << Color::BOLD
              << "  Total Credits  : " << totalCredits() << "\n"
              << "  Average Marks  : " << std::fixed << std::setprecision(2) << calculateAverage() << "\n"
              << "  Cumulative GPA : " << Color::CYAN
              << std::setprecision(2) << calculateGPA()
              << Color::RESET << "  [" << academicStanding() << "]\n\n";
}

std::string Student::escapeCsv(const std::string& s) {
    if (s.find_first_of(",\"\n") == std::string::npos) return s;
    std::string out = "\"";
    for (char ch : s) { if (ch == '"') out += '"'; out += ch; }
    out += '"';
    return out;
}

std::string Student::unescapeCsv(const std::string& s) {
    if (s.size() < 2 || s.front() != '"') return s;
    std::string out;
    for (size_t i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == '"' && s[i+1] == '"') { out += '"'; ++i; }
        else out += s[i];
    }
    return out;
}

std::string Student::toCSV() const {
    std::ostringstream ss;
    ss << m_id              << ","
       << escapeCsv(m_name) << ","
       << m_age             << ","
       << m_gender          << ","
       << m_email           << ","
       << m_phone           << ","
       << m_department      << ","
       << m_semester        << ","
       << m_enrollDate      << ","
       << (m_active ? 1 : 0);

    for (const auto& c : m_courses) {
        ss << ";" << c.code << "|" << escapeCsv(c.name)
           << "|" << c.marks << "|" << c.credits;
    }
    return ss.str();
}

Student Student::fromCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::string chunk;
    bool inQuote = false;
    size_t commaCount = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];
        if (ch == '"') { inQuote = !inQuote; chunk += ch; }
        else if (ch == ',' && !inQuote && commaCount < 9) {
            fields.push_back(unescapeCsv(chunk));
            chunk.clear();
            ++commaCount;
        } else chunk += ch;
    }
    fields.push_back(chunk);

    if (fields.size() < 10)
        throw std::runtime_error("Malformed CSV record.");

    Student s;
    s.m_id         = std::stoi(fields[0]);
    s.m_name       = fields[1];
    s.m_age        = std::stoi(fields[2]);
    s.m_gender     = fields[3];
    s.m_email      = fields[4];
    s.m_phone      = fields[5];
    s.m_department = fields[6];
    s.m_semester   = std::stoi(fields[7]);
    s.m_enrollDate = fields[8];

    std::string rest = fields[9];
    size_t semi = rest.find(';');
    s.m_active = (rest.substr(0, semi) == "1");

    if (semi != std::string::npos) {
        std::string courseStr = rest.substr(semi + 1);
        std::istringstream css(courseStr);
        std::string token;
        while (std::getline(css, token, ';')) {
            std::istringstream ts(token);
            std::vector<std::string> parts;
            std::string p;
            while (std::getline(ts, p, '|')) parts.push_back(p);
            if (parts.size() == 4) {
                Course c;
                c.code    = parts[0];
                c.name    = unescapeCsv(parts[1]);
                c.marks   = std::stod(parts[2]);
                c.credits = std::stoi(parts[3]);
                s.addCourse(c);
            }
        }
    }
    return s;
}
