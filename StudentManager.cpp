#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "StudentManager.h"
#include "Common.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <map>
#include <numeric>
#include <cmath>
#include <cctype>

StudentManager::StudentManager() : m_nextId(1001) {}

int StudentManager::addStudent(const Student& s) {
    Student ns = s;
    if (ns.getId() == 0) {
        Student tmp(m_nextId, ns.getName(), ns.getAge(), ns.getGender(),
                    ns.getEmail(), ns.getPhone(), ns.getDepartment(),
                    ns.getSemester(), ns.getEnrollDate());
        for (const auto& c : ns.getCourses()) tmp.addCourse(c);
        m_students.push_back(tmp);
        m_nextId++;
    } else {
        if (idExists(ns.getId()))
            throw std::runtime_error("Student ID " + std::to_string(ns.getId()) + " already exists.");
        m_students.push_back(ns);
        if (ns.getId() >= m_nextId) m_nextId = ns.getId() + 1;
    }
    return m_students.back().getId();
}

bool StudentManager::updateStudent(int id, const Student& updated) {
    Student* s = findById(id);
    if (!s) return false;
    s->setName(updated.getName());
    s->setAge(updated.getAge());
    s->setGender(updated.getGender());
    s->setEmail(updated.getEmail());
    s->setPhone(updated.getPhone());
    s->setDepartment(updated.getDepartment());
    s->setSemester(updated.getSemester());
    s->setEnrollDate(updated.getEnrollDate());
    return true;
}

bool StudentManager::deleteStudent(int id) {
    Student* s = findById(id);
    if (!s || !s->isActive()) return false;
    s->setActive(false);
    return true;
}

bool StudentManager::restoreStudent(int id) {
    Student* s = findById(id);
    if (!s || s->isActive()) return false;
    s->setActive(true);
    return true;
}

bool StudentManager::hardDeleteStudent(int id) {
    auto it = std::find_if(m_students.begin(), m_students.end(),
                           [id](const Student& s){ return s.getId() == id; });
    if (it == m_students.end()) return false;
    m_students.erase(it);
    return true;
}

Student* StudentManager::findById(int id) {
    auto it = std::find_if(m_students.begin(), m_students.end(),
                           [id](const Student& s){ return s.getId() == id; });
    return (it != m_students.end()) ? &(*it) : nullptr;
}

const Student* StudentManager::findById(int id) const {
    auto it = std::find_if(m_students.begin(), m_students.end(),
                           [id](const Student& s){ return s.getId() == id; });
    return (it != m_students.end()) ? &(*it) : nullptr;
}

std::vector<Student*> StudentManager::search(const SearchQuery& q) {
    std::vector<Student*> results;
    for (auto& s : m_students) {
        bool match = false;
        switch (q.field) {
            case SearchField::ID:
                match = (std::to_string(s.getId()) == q.value);
                break;
            case SearchField::NAME_CONTAINS: {
                std::string name  = s.getName(), val = q.value;
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                std::transform(val.begin(),  val.end(),  val.begin(),  ::tolower);
                match = (name.find(val) != std::string::npos);
                break;
            }
            case SearchField::DEPARTMENT: {
                std::string dept = s.getDepartment(), val = q.value;
                std::transform(dept.begin(), dept.end(), dept.begin(), ::tolower);
                std::transform(val.begin(),  val.end(),  val.begin(),  ::tolower);
                match = (dept.find(val) != std::string::npos);
                break;
            }
            case SearchField::EMAIL: {
                std::string email = s.getEmail(), val = q.value;
                std::transform(email.begin(), email.end(), email.begin(), ::tolower);
                std::transform(val.begin(),   val.end(),   val.begin(),   ::tolower);
                match = (email.find(val) != std::string::npos);
                break;
            }
            case SearchField::PHONE: {
                std::string phone = s.getPhone(), val = q.value;
                match = (phone.find(val) != std::string::npos);
                break;
            }
            case SearchField::COURSE_CODE: {
                match = s.hasCourse(q.value);
                break;
            }
        }
        if (match) results.push_back(&s);
    }
    return results;
}

std::vector<Student*> StudentManager::filter(std::function<bool(const Student&)> pred) {
    std::vector<Student*> results;
    for (auto& s : m_students)
        if (pred(s)) results.push_back(&s);
    return results;
}

std::vector<const Student*> StudentManager::filter(std::function<bool(const Student&)> pred) const {
    std::vector<const Student*> results;
    for (const auto& s : m_students)
        if (pred(s)) results.push_back(&s);
    return results;
}

std::vector<const Student*> StudentManager::sorted(SortField sf, SortOrder so,
                                                    bool activeOnly) const {
    std::vector<const Student*> ptrs;
    for (const auto& s : m_students)
        if (!activeOnly || s.isActive()) ptrs.push_back(&s);

    auto cmp = [&](const Student* a, const Student* b) -> bool {
        if (so == SortOrder::ASCENDING) {
            switch (sf) {
                case SortField::ID:         return a->getId()         < b->getId();
                case SortField::NAME:       return a->getName()       < b->getName();
                case SortField::GPA:        return a->calculateGPA()  < b->calculateGPA();
                case SortField::DEPARTMENT: return a->getDepartment() < b->getDepartment();
                case SortField::SEMESTER:   return a->getSemester()   < b->getSemester();
                case SortField::AGE:        return a->getAge()        < b->getAge();
            }
        } else {
            switch (sf) {
                case SortField::ID:         return a->getId()         > b->getId();
                case SortField::NAME:       return a->getName()       > b->getName();
                case SortField::GPA:        return a->calculateGPA()  > b->calculateGPA();
                case SortField::DEPARTMENT: return a->getDepartment() > b->getDepartment();
                case SortField::SEMESTER:   return a->getSemester()   > b->getSemester();
                case SortField::AGE:        return a->getAge()        > b->getAge();
            }
        }
        return false;
    };
    std::sort(ptrs.begin(), ptrs.end(), cmp);
    return ptrs;
}

void StudentManager::listAll(SortField sf, SortOrder so, bool activeOnly) const {
    auto ptrs = sorted(sf, so, activeOnly);
    if (ptrs.empty()) {
        std::cout << Color::YELLOW << "  No students found.\n" << Color::RESET;
        return;
    }
    std::cout << "\n" << Color::BOLD << Color::WHITE
              << "  " << std::left
              << std::setw(6)  << "ID"
              << std::setw(24) << "Name"
              << std::setw(18) << "Department"
              << std::setw(5)  << "Sem"
              << std::setw(10) << "GPA"
              << "Status" << Color::RESET << "\n"
              << "  " << std::string(70, '-') << "\n";
    for (const auto* s : ptrs) s->printSummary();
    std::cout << "  " << std::string(70, '-') << "\n"
              << Color::DIM << "  Showing " << ptrs.size() << " record(s).\n"
              << Color::RESET;
}

void StudentManager::listByDept(const std::string& dept) const {
    auto results = filter([&](const Student& s){
        std::string sDept = s.getDepartment();
        std::string dQuery = dept;
        std::transform(sDept.begin(), sDept.end(), sDept.begin(), ::tolower);
        std::transform(dQuery.begin(), dQuery.end(), dQuery.begin(), ::tolower);
        return s.isActive() && sDept.find(dQuery) != std::string::npos;
    });

    if (results.empty()) {
        std::cout << Color::YELLOW << "  No active students found in department: " << dept << "\n" << Color::RESET;
        return;
    }

    std::cout << "\n" << Color::BOLD << "  Students in " << Color::CYAN << dept << Color::RESET << "\n"
              << "  " << std::string(70, '-') << "\n";
    for (const auto* s : results) s->printSummary();
    std::cout << "  " << std::string(70, '-') << "\n";
}

void StudentManager::listTopN(int n) const {
    auto ptrs = sorted(SortField::GPA, SortOrder::DESCENDING, true);
    if ((int)ptrs.size() < n) n = (int)ptrs.size();
    std::cout << "\n" << Color::BOLD << Color::YELLOW
              << "  🏆 Top " << n << " Students by GPA\n" << Color::RESET
              << "  " << std::string(70, '-') << "\n";
    int rank = 1;
    for (int i = 0; i < n; ++i) {
        std::cout << Color::BOLD << "  #" << rank++ << "  " << Color::RESET;
        ptrs[i]->printSummary();
    }
}
void StudentManager::printStats() const {
    auto active = filter([](const Student& s){ return s.isActive(); });
    if (active.empty()) { 
        std::cout << Color::YELLOW << "\n  [i] No active students found to calculate statistics.\n" << Color::RESET; 
        return; 
    }
    double totalGPA = 0.0;
    double maxGPA   = 0.0, minGPA = 4.0;
    const Student *topS = nullptr, *botS = nullptr;
    for (const auto* s : active) {
        double g = s->calculateGPA();
        totalGPA += g;
        if (g > maxGPA) { maxGPA = g; topS = s; }
        if (g < minGPA) { minGPA = g; botS = s; }
    }
    double avgGPA = totalGPA / active.size();
    double variance = 0.0;
    for (const auto* s : active) {
        double diff = s->calculateGPA() - avgGPA;
        variance += diff * diff;
    }
    double stddev = std::sqrt(variance / active.size());
    std::cout << "\n" << Color::BOLD << Color::CYAN
              << "  ╔══ SYSTEM STATISTICS ════════════════════════════════╗\n" << Color::RESET;
    auto stat = [](const std::string& label, const std::string& val){
        std::cout << Color::CYAN << "  ║  " << Color::BOLD << std::left
                  << std::setw(22) << label << Color::RESET << ": " << val << "\n";
    };
    stat("Total Students",   std::to_string(active.size()));
    stat("Average GPA",      std::to_string(avgGPA).substr(0,5));
    stat("Highest GPA",      (topS ? std::to_string(maxGPA).substr(0,5)+" ("+topS->getName()+")" : "-"));
    stat("Lowest GPA",       (botS ? std::to_string(minGPA).substr(0,5)+" ("+botS->getName()+")" : "-"));
    stat("GPA Std-Dev",      std::to_string(stddev).substr(0,5));
    std::cout << Color::CYAN << Color::BOLD
              << "  ╚═════════════════════════════════════════════════════╝\n"
              << Color::RESET;
}

void StudentManager::printDeptStats() const {
    std::map<std::string, std::vector<double>> deptGPAs;
    for (const auto& s : m_students)
        if (s.isActive())
            deptGPAs[s.getDepartment()].push_back(s.calculateGPA());

    std::cout << "\n" << Color::BOLD << Color::WHITE
              << "  " << std::left << std::setw(26) << "Department"
              << std::setw(10) << "Students" << std::setw(10) << "Avg GPA"
              << Color::RESET << "\n  " << std::string(46, '-') << "\n";

    for (const auto& [dept, gpas] : deptGPAs) {
        double avg = std::accumulate(gpas.begin(), gpas.end(), 0.0) / gpas.size();
        std::cout << "  " << std::left << std::setw(26) << dept
                  << std::setw(10) << gpas.size()
                  << std::fixed << std::setprecision(2) << avg << "\n";
    }
}

int StudentManager::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Cannot open file for writing: " + filename);
    int count = 0;
    for (const auto& s : m_students) {
        ofs << s.toCSV() << "\n";
        ++count;
    }
    return count;
}

int StudentManager::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) return 0;
    m_students.clear();
    m_nextId = 1001;
    std::string line;
    int count = 0;
    int lineNum = 0;
    while (std::getline(ifs, line)) {
        ++lineNum;
        if (line.empty()) continue;
        try {
            Student s = Student::fromCSV(line);
            m_students.push_back(s);
            if (s.getId() >= m_nextId) m_nextId = s.getId() + 1;
            ++count;
        } catch (const std::exception& e) {
            std::cerr << Color::YELLOW << "  Warning: skipping line " << lineNum
                      << ": " << e.what() << Color::RESET << "\n";
        }
    }
    return count;
}

void StudentManager::exportCSV(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Cannot open: " + filename);
    ofs << "ID,Name,Age,Gender,Email,Phone,Department,Semester,EnrollDate,Active,GPA,Standing\n";
    for (const auto& s : m_students) {
        ofs << s.getId()          << ","
            << s.getName()        << ","
            << s.getAge()         << ","
            << s.getGender()      << ","
            << s.getEmail()       << ","
            << s.getPhone()       << ","
            << s.getDepartment()  << ","
            << s.getSemester()    << ","
            << s.getEnrollDate()  << ","
            << (s.isActive() ? "Yes" : "No") << ","
            << std::fixed << std::setprecision(2) << s.calculateGPA() << ","
            << s.academicStanding() << "\n";
    }
}

size_t StudentManager::count(bool activeOnly) const {
    if (!activeOnly) return m_students.size();
    return std::count_if(m_students.begin(), m_students.end(),
                         [](const Student& s){ return s.isActive(); });
}

int StudentManager::nextId() const { return m_nextId; }

bool StudentManager::idExists(int id) const {
    return std::any_of(m_students.begin(), m_students.end(),
                       [id](const Student& s){ return s.getId() == id; });
}
