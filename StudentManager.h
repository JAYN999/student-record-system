#pragma once
#include "Student.h"
#include <vector>
#include <string>
#include <functional>
#include <optional>

enum class SortField  { ID, NAME, GPA, DEPARTMENT, SEMESTER, AGE };
enum class SortOrder  { ASCENDING, DESCENDING };
enum class SearchField{ ID, NAME_CONTAINS, DEPARTMENT, EMAIL, PHONE, COURSE_CODE };

struct SearchQuery {
    SearchField field;
    std::string value;
};

class StudentManager {
public:
    StudentManager();

    int  addStudent(const Student& s);
    bool updateStudent(int id, const Student& s);
    bool deleteStudent(int id);
    bool restoreStudent(int id);
    bool hardDeleteStudent(int id);

    Student*              findById(int id);
    const Student*        findById(int id) const;
    std::vector<Student*>       search(const SearchQuery& q);
    std::vector<Student*>       filter(std::function<bool(const Student&)> pred);
    std::vector<const Student*> filter(std::function<bool(const Student&)> pred) const;

    void listAll     (SortField sf = SortField::ID,
                      SortOrder so = SortOrder::ASCENDING,
                      bool activeOnly = true) const;
    void listByDept  (const std::string& dept) const;
    void listTopN    (int n) const;

    void printStats()    const;
    void printDeptStats()const;

    void importCSV (const std::string& filename);
    void exportCSV (const std::string& filename) const;
    int  loadFromFile(const std::string& filename);
    int  saveToFile  (const std::string& filename) const;

    size_t count(bool activeOnly = true) const;
    int    nextId() const;
    bool   idExists(int id) const;

private:
    std::vector<Student> m_students;
    int                  m_nextId;

    std::vector<const Student*> sorted(SortField sf, SortOrder so,
                                       bool activeOnly) const;
};
