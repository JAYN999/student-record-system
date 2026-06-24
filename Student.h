#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

struct Course {
    std::string code;
    std::string name;
    double      marks;
    int         credits;

    Course() : marks(0.0), credits(0) {}
    Course(const std::string& c, const std::string& n, double m, int cr)
        : code(c), name(n), marks(m), credits(cr) {}

    std::string letterGrade() const {
        if (marks >= 90) return "A+";
        if (marks >= 85) return "A";
        if (marks >= 80) return "A-";
        if (marks >= 75) return "B+";
        if (marks >= 70) return "B";
        if (marks >= 65) return "B-";
        if (marks >= 60) return "C+";
        if (marks >= 55) return "C";
        if (marks >= 50) return "C-";
        if (marks >= 45) return "D";
        return "F";
    }

    double gradePoint() const {
        if (marks >= 90) return 4.0;
        if (marks >= 85) return 4.0;
        if (marks >= 80) return 3.7;
        if (marks >= 75) return 3.3;
        if (marks >= 70) return 3.0;
        if (marks >= 65) return 2.7;
        if (marks >= 60) return 2.3;
        if (marks >= 55) return 2.0;
        if (marks >= 50) return 1.7;
        if (marks >= 45) return 1.0;
        return 0.0;
    }
};

class Student {
public:
    Student();
    Student(int id, const std::string& name, int age,
            const std::string& gender, const std::string& email,
            const std::string& phone, const std::string& department,
            int semester, const std::string& enrollDate);

    int                         getId()         const { return m_id; }
    const std::string&          getName()       const { return m_name; }
    int                         getAge()        const { return m_age; }
    const std::string&          getGender()     const { return m_gender; }
    const std::string&          getEmail()      const { return m_email; }
    const std::string&          getPhone()      const { return m_phone; }
    const std::string&          getDepartment() const { return m_department; }
    int                         getSemester()   const { return m_semester; }
    const std::string&          getEnrollDate() const { return m_enrollDate; }
    const std::vector<Course>&  getCourses()    const { return m_courses; }
    bool                        isActive()      const { return m_active; }

    void setName(const std::string& v)       { m_name = v; }
    void setAge(int v)                       { m_age = v; }
    void setGender(const std::string& v)     { m_gender = v; }
    void setEmail(const std::string& v)      { m_email = v; }
    void setPhone(const std::string& v)      { m_phone = v; }
    void setDepartment(const std::string& v) { m_department = v; }
    void setSemester(int v)                  { m_semester = v; }
    void setEnrollDate(const std::string& v) { m_enrollDate = v; }
    void setActive(bool v)                   { m_active = v; }

    void   addCourse(const Course& c);
    bool   updateCourse(const std::string& code, double newMarks);
    bool   removeCourse(const std::string& code);
    bool   hasCourse(const std::string& code) const;

    double calculateGPA()     const;
    double calculateAverage() const;
    int    totalCredits()     const;
    std::string academicStanding() const;

    void printSummary()    const;
    void printDetailed()   const;
    void printTranscript() const;

    std::string toCSV()           const;
    static Student fromCSV(const std::string& line);

private:
    int                  m_id;
    std::string          m_name;
    int                  m_age;
    std::string          m_gender;
    std::string          m_email;
    std::string          m_phone;
    std::string          m_department;
    int                  m_semester;
    std::string          m_enrollDate;
    std::vector<Course>  m_courses;
    bool                 m_active;

    static std::string escapeCsv(const std::string& s);
    static std::string unescapeCsv(const std::string& s);
};
