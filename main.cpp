#include "Common.h"
#include "Student.h"
#include "StudentManager.h"
#include "Utils.h"
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

const std::string DATA_FILE = "students.dat";

static void printBanner() {
  std::cout << Color::CYAN << Color::BOLD << R"(
  ╔══════════════════════════════════════════════════════════════════════════╗
  ║                                                                          ║
  ║  ███████╗ ████████╗ ██╗   ██╗ ██████╗  ███████╗ ███╗  ██╗ ████████╗      ║
  ║  ██╔════╝ ╚══██╔══╝ ██║   ██║ ██╔══██╗ ██╔════╝ ████╗ ██║ ╚══██╔══╝      ║
  ║  ███████╗    ██║    ██║   ██║ ██║  ██║ █████╗   ██╔██╗██║    ██║         ║
  ║  ╚════██║    ██║    ██║   ██║ ██║  ██║ ██╔══╝   ██║╚████║    ██║         ║
  ║  ███████║    ██║    ╚██████╔╝ ██████╔╝ ███████╗ ██║ ╚███║    ██║         ║
  ║  ╚══════╝    ╚═╝     ╚═════╝  ╚═════╝  ╚══════╝ ╚═╝  ╚══╝    ╚═╝         ║
  ║                                                                          ║
  ║            R E C O R D    M A N A G E M E N T    S Y S T E M             ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;
}

static void printMainMenu() {
  std::cout << "\n"
            << Color::BOLD << Color::WHITE
            << "  ╔══ MAIN MENU ═════════════════════════════╗\n"
            << "  ║  " << Color::CYAN << "1" << Color::WHITE
            << "  Add New Student                    ║\n"
            << "  ║  " << Color::CYAN << "2" << Color::WHITE
            << "  View / Search Students             ║\n"
            << "  ║  " << Color::CYAN << "3" << Color::WHITE
            << "  Edit Student Record                ║\n"
            << "  ║  " << Color::CYAN << "4" << Color::WHITE
            << "  Manage Courses & Marks             ║\n"
            << "  ║  " << Color::CYAN << "5" << Color::WHITE
            << "  Delete / Restore Student           ║\n"
            << "  ║  " << Color::CYAN << "6" << Color::WHITE
            << "  Reports & Statistics               ║\n"
            << "  ║  " << Color::CYAN << "7" << Color::WHITE
            << "  Export / Backup Data               ║\n"
            << "  ║  " << Color::RED << "0" << Color::WHITE
            << "  Exit                               ║\n"
            << "  ╚═════════════════════════════════════════╝\n"
            << Color::RESET;
}

static void printCourseMenu() {
  std::cout << "\n"
            << Color::BOLD << Color::WHITE
            << "  ╔══ COURSE MENU ═══════════════════════════╗\n"
            << "  ║  " << Color::CYAN << "1" << Color::WHITE
            << "  Add Course                         ║\n"
            << "  ║  " << Color::CYAN << "2" << Color::WHITE
            << "  Update Marks                       ║\n"
            << "  ║  " << Color::CYAN << "3" << Color::WHITE
            << "  Remove Course                      ║\n"
            << "  ║  " << Color::CYAN << "4" << Color::WHITE
            << "  View Transcript                    ║\n"
            << "  ║  " << Color::RED << "0" << Color::WHITE
            << "  Back                               ║\n"
            << "  ╚═════════════════════════════════════════╝\n"
            << Color::RESET;
}

static void printReportMenu() {
  std::cout << "\n"
            << Color::BOLD << Color::WHITE
            << "  ╔══ REPORTS ═══════════════════════════════╗\n"
            << "  ║  " << Color::CYAN << "1" << Color::WHITE
            << "  All Students (sorted by GPA ↓)     ║\n"
            << "  ║  " << Color::CYAN << "2" << Color::WHITE
            << "  Top 10 Students                    ║\n"
            << "  ║  " << Color::CYAN << "3" << Color::WHITE
            << "  Department Statistics              ║\n"
            << "  ║  " << Color::CYAN << "4" << Color::WHITE
            << "  System Statistics                  ║\n"
            << "  ║  " << Color::CYAN << "5" << Color::WHITE
            << "  Students on Probation / Suspension ║\n"
            << "  ║  " << Color::RED << "0" << Color::WHITE
            << "  Back                               ║\n"
            << "  ╚═════════════════════════════════════════╝\n"
            << Color::RESET;
}

static Student collectStudentData(int id) {
  std::cout << "\n"
            << Color::BOLD << Color::CYAN
            << "  ── Please enter student details ──────────────────────\n"
            << Color::RESET;

  std::string name = Utils::getLine("Full Name       : ");

  int age = Utils::getInt("Age (15-80)     : ", 15, 80);

  char gc = Utils::getChar("Gender [m/f/o]  : ", "mfo");
  std::string gender = (gc == 'm') ? "Male" : (gc == 'f') ? "Female" : "Other";

  std::string email;
  while (true) {
    email = Utils::getLine("Email           : ");
    if (Utils::isValidEmail(email))
      break;
    std::cout << Color::RED << "  Invalid email format. Try again.\n"
              << Color::RESET;
  }

  std::string phone;
  while (true) {
    phone = Utils::getLine("Phone           : ");
    if (Utils::isValidPhone(phone))
      break;
    std::cout << Color::RED
              << "  Invalid phone. Use digits/spaces/+/- (7-15 chars).\n"
              << Color::RESET;
  }

  std::string dept = Utils::getLine("Department      : ");
  int semester = Utils::getInt("Semester (1-10) : ", 1, 10);

  std::string enrollDate;
  std::cout << "  Enroll Date [YYYY-MM-DD, blank = today]: ";
  std::getline(std::cin, enrollDate);
  enrollDate = Utils::trim(enrollDate);
  if (enrollDate.empty())
    enrollDate = Utils::currentDate();
  while (!Utils::isValidDate(enrollDate)) {
    std::cout << Color::RED
              << "  Invalid date (use YYYY-MM-DD). Try again: " << Color::RESET;
    std::getline(std::cin, enrollDate);
    enrollDate = Utils::trim(enrollDate);
  }

  return Student(id, name, age, gender, email, phone, dept, semester,
                 enrollDate);
}

static void featureAddStudent(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN << "\n  ╔══ ADD NEW STUDENT ═══╗\n"
            << Color::RESET;

  try {
    Student s = collectStudentData(0);
    int newId = mgr.addStudent(s);
    std::cout << Color::GREEN
              << "\n  ✔ Student added successfully! ID: " << Color::BOLD
              << newId << Color::RESET << "\n";

    if (Utils::getYesNo("\n  Add courses now?")) {
      Student *sp = mgr.findById(newId);
      while (true) {
        std::cout << "\n";
        std::string code =
            Utils::getLine("  Course Code (blank to stop): ", false);
        if (code.empty())
          break;
        std::string cname = Utils::getLine("  Course Name   : ");
        int credits = Utils::getInt("  Credits (1-6) : ", 1, 6);
        double marks = Utils::getDouble("  Marks (0-100) : ", 0, 100);
        try {
          sp->addCourse(Course(code, cname, marks, credits));
          std::cout << Color::GREEN << "  ✔ Course added.\n" << Color::RESET;
        } catch (const std::exception &e) {
          std::cout << Color::RED << "  ✘ " << e.what() << "\n" << Color::RESET;
        }
      }
    }
    mgr.saveToFile(DATA_FILE);
  } catch (const std::exception &e) {
    std::cout << Color::RED << "\n  Error: " << e.what() << "\n"
              << Color::RESET;
  }
  Utils::pauseScreen();
}

static void featureViewSearch(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN
            << "\n  ╔══ VIEW / SEARCH ════════════════════════╗\n"
            << Color::RESET << "  1. List all students\n"
            << "  2. Search by ID\n"
            << "  3. Search by Name\n"
            << "  4. Search by Department\n"
            << "  5. Search by Email\n"
            << "  6. Search by Phone\n"
            << "  7. Search by Course Code\n"
            << "  8. Search by GPA Range\n"
            << "  9. Search by Age Range\n"
            << "  10. View Full Profile + Transcript (by ID)\n"
            << "  11. List inactive / deleted students\n"
            << "  0. Back\n";

  int choice = Utils::getInt("\n  Choice: ", 0, 11);
  std::cout << "\n";

  switch (choice) {
  case 1:
    std::cout << Color::BOLD << "  Sort by: 1=ID  2=Name  3=GPA  4=Dept\n"
              << Color::RESET;
    {
      int sf = Utils::getInt("  Sort field: ", 1, 4);
      SortField sortF = (sf == 1)   ? SortField::ID
                        : (sf == 2) ? SortField::NAME
                        : (sf == 3) ? SortField::GPA
                                    : SortField::DEPARTMENT;
      char ord = Utils::getChar("  Order [a=asc / d=desc]: ", "ad");
      SortOrder sortO =
          (ord == 'a') ? SortOrder::ASCENDING : SortOrder::DESCENDING;
      mgr.listAll(sortF, sortO);
    }
    break;
  case 2: {
    int id = Utils::getInt("  Student ID: ", 1, 999999);
    const Student *s = mgr.findById(id);
    if (s)
      s->printSummary();
    else
      std::cout << Color::RED << "  Student not found.\n" << Color::RESET;
    break;
  }
  case 3: {
    std::string name = Utils::getLine("  Name (partial): ");
    auto results = mgr.search({SearchField::NAME_CONTAINS, name});
    if (results.empty())
      std::cout << Color::YELLOW << "  No results.\n" << Color::RESET;
    else
      for (auto *s : results)
        s->printSummary();
    break;
  }
  case 4: {
    std::string dept = Utils::getLine("  Department (partial): ");
    mgr.listByDept(dept);
    break;
  }
  case 5: {
    std::string email = Utils::getLine("  Email (partial): ");
    auto results = mgr.search({SearchField::EMAIL, email});
    if (results.empty())
      std::cout << Color::YELLOW << "  No students found with email matching '"
                << email << "'.\n"
                << Color::RESET;
    else
      for (auto *s : results)
        s->printSummary();
    break;
  }
  case 6: {
    std::string phone = Utils::getLine("  Phone (partial): ");
    auto results = mgr.search({SearchField::PHONE, phone});
    if (results.empty())
      std::cout << Color::YELLOW << "  No students found with phone matching '"
                << phone << "'.\n"
                << Color::RESET;
    else
      for (auto *s : results)
        s->printSummary();
    break;
  }
  case 7: {
    std::string code = Utils::getLine("  Course Code: ");
    auto results = mgr.search({SearchField::COURSE_CODE, code});
    if (results.empty())
      std::cout << Color::YELLOW << "  No students enrolled in course '" << code
                << "'.\n"
                << Color::RESET;
    else {
      std::cout << Color::BOLD << "  Students enrolled in " << Color::CYAN
                << code << Color::RESET << ":\n";
      for (auto *s : results)
        s->printSummary();
    }
    break;
  }
  case 8: {
    double mn = Utils::getDouble("  Min GPA: ", 0.0, 4.0);
    double mx = Utils::getDouble("  Max GPA: ", mn, 4.0);
    auto results = mgr.filter([&](const Student &s) {
      double g = s.calculateGPA();
      return s.isActive() && g >= mn && g <= mx;
    });
    if (results.empty())
      std::cout << Color::YELLOW << "  No students in range [" << mn << " - "
                << mx << "].\n"
                << Color::RESET;
    else
      for (auto *s : results)
        s->printSummary();
    break;
  }
  case 9: {
    int mn = Utils::getInt("  Min Age: ", 15, 80);
    int mx = Utils::getInt("  Max Age: ", mn, 80);
    auto results = mgr.filter([&](const Student &s) {
      return s.isActive() && s.getAge() >= mn && s.getAge() <= mx;
    });
    if (results.empty())
      std::cout << Color::YELLOW << "  No students in age range [" << mn
                << " - " << mx << "].\n"
                << Color::RESET;
    else
      for (auto *s : results)
        s->printSummary();
    break;
  }
  case 10: {
    int id = Utils::getInt("  Student ID: ", 1, 999999);
    const Student *s = mgr.findById(id);
    if (s)
      s->printTranscript();
    else
      std::cout << Color::RED << "  Student not found.\n" << Color::RESET;
    break;
  }
  case 11:
    mgr.listAll(SortField::ID, SortOrder::ASCENDING, false);
    break;
  default:
    return;
  }
  Utils::pauseScreen();
}

static void featureEditStudent(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN << "\n  ╔══ EDIT STUDENT ══════╗\n"
            << Color::RESET;

  int id = Utils::getInt("  Student ID to edit: ", 1, 999999);
  Student *s = mgr.findById(id);
  if (!s) {
    std::cout << Color::RED << "  Student not found.\n" << Color::RESET;
    Utils::pauseScreen();
    return;
  }
  s->printDetailed();
  std::cout << Color::YELLOW
            << "\n  Enter new details (leave blank to keep current value):\n"
            << Color::RESET;

  auto optLine = [&](const std::string &prompt,
                     const std::string &current) -> std::string {
    std::cout << "  " << prompt << " [" << current << "]: ";
    std::string v;
    std::getline(std::cin, v);
    v = Utils::trim(v);
    return v.empty() ? current : v;
  };
  auto optInt = [&](const std::string &prompt, int current, int mn,
                    int mx) -> int {
    std::cout << "  " << prompt << " [" << current << "]: ";
    std::string v;
    std::getline(std::cin, v);
    v = Utils::trim(v);
    if (v.empty())
      return current;
    try {
      int val = std::stoi(v);
      if (val >= mn && val <= mx)
        return val;
    } catch (...) {
    }
    return current;
  };

  std::string name = optLine("Name", s->getName());
  int age = optInt("Age", s->getAge(), 15, 80);
  std::string dept = optLine("Department", s->getDepartment());
  int sem = optInt("Semester", s->getSemester(), 1, 10);
  std::string email = optLine("Email", s->getEmail());
  std::string phone = optLine("Phone", s->getPhone());

  Student updated(s->getId(), name, age, s->getGender(), email, phone, dept,
                  sem, s->getEnrollDate());
  mgr.updateStudent(id, updated);
  mgr.saveToFile(DATA_FILE);

  std::cout << Color::GREEN << "\n  ✔ Student record updated.\n"
            << Color::RESET;
  Utils::pauseScreen();
}

static void featureManageCourses(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN << "\n  ╔══ COURSE MANAGEMENT ══╗\n"
            << Color::RESET;

  int id = Utils::getInt("  Student ID: ", 1, 999999);
  Student *s = mgr.findById(id);
  if (!s) {
    std::cout << Color::RED << "  Student not found.\n" << Color::RESET;
    Utils::pauseScreen();
    return;
  }

  bool running = true;
  while (running) {
    Utils::clearScreen();
    s->printTranscript();
    printCourseMenu();
    int choice = Utils::getInt("  Choice: ", 0, 4);

    switch (choice) {
    case 1: {
      std::string code = Utils::getLine("  Course Code : ");
      std::string cname = Utils::getLine("  Course Name : ");
      int credits = Utils::getInt("  Credits     : ", 1, 6);
      double marks = Utils::getDouble("  Marks       : ", 0, 100);
      try {
        s->addCourse(Course(code, cname, marks, credits));
        mgr.saveToFile(DATA_FILE);
        std::cout << Color::GREEN << "  ✔ Course enrolled.\n" << Color::RESET;
      } catch (const std::exception &e) {
        std::cout << Color::RED << "  ✘ " << e.what() << "\n" << Color::RESET;
      }
      break;
    }
    case 2: {
      std::string code = Utils::getLine("  Course Code to update: ");
      double marks = Utils::getDouble("  New Marks: ", 0, 100);
      try {
        if (s->updateCourse(code, marks)) {
          mgr.saveToFile(DATA_FILE);
          std::cout << Color::GREEN << "  ✔ Marks updated.\n" << Color::RESET;
        } else
          std::cout << Color::RED << "  Course not found.\n" << Color::RESET;
      } catch (const std::exception &e) {
        std::cout << Color::RED << "  ✘ " << e.what() << "\n" << Color::RESET;
      }
      break;
    }
    case 3: {
      std::string code = Utils::getLine("  Course Code to remove: ");
      if (s->removeCourse(code)) {
        mgr.saveToFile(DATA_FILE);
        std::cout << Color::GREEN << "  ✔ Course removed.\n" << Color::RESET;
      } else
        std::cout << Color::RED << "  Course not found.\n" << Color::RESET;
      break;
    }
    case 4:
      s->printTranscript();
      break;
    case 0:
      running = false;
      break;
    }
    if (choice != 0)
      Utils::pauseScreen();
  }
}

static void featureDeleteRestore(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN << "\n  ╔══ DELETE / RESTORE ══╗\n"
            << Color::RESET << "  1. Soft-delete student\n"
            << "  2. Restore student\n"
            << "  3. Permanently remove student\n"
            << "  0. Back\n";

  int choice = Utils::getInt("\n  Choice: ", 0, 3);
  if (choice == 0)
    return;

  int id = Utils::getInt("  Student ID: ", 1, 999999);

  switch (choice) {
  case 1:
    if (mgr.deleteStudent(id)) {
      mgr.saveToFile(DATA_FILE);
      std::cout << Color::GREEN << "  ✔ Student marked as inactive.\n"
                << Color::RESET;
    } else
      std::cout << Color::RED << "  ✘ Not found or already inactive.\n"
                << Color::RESET;
    break;
  case 2:
    if (mgr.restoreStudent(id)) {
      mgr.saveToFile(DATA_FILE);
      std::cout << Color::GREEN << "  ✔ Student restored.\n" << Color::RESET;
    } else
      std::cout << Color::RED << "  ✘ Not found or already active.\n"
                << Color::RESET;
    break;
  case 3:
    if (Utils::getYesNo("  This is PERMANENT. Confirm?")) {
      if (mgr.hardDeleteStudent(id)) {
        mgr.saveToFile(DATA_FILE);
        std::cout << Color::GREEN << "  ✔ Student permanently deleted.\n"
                  << Color::RESET;
      } else
        std::cout << Color::RED << "  ✘ Student not found.\n" << Color::RESET;
    } else
      std::cout << "  Cancelled.\n";
    break;
  }
  Utils::pauseScreen();
}

static void featureReports(StudentManager &mgr) {
  bool running = true;
  while (running) {
    Utils::clearScreen();
    printReportMenu();
    int choice = Utils::getInt("  Choice: ", 0, 5);

    switch (choice) {
    case 1:
      mgr.listAll(SortField::GPA, SortOrder::DESCENDING);
      break;
    case 2:
      mgr.listTopN(10);
      break;
    case 3:
      mgr.printDeptStats();
      break;
    case 4:
      mgr.printStats();
      break;
    case 5: {
      std::cout << Color::BOLD
                << "\n  Students on Academic Probation / Suspension:\n"
                << Color::RESET;
      auto at_risk = mgr.filter([](const Student &s) {
        auto standing = s.academicStanding();
        return s.isActive() && (standing == "Academic Probation" ||
                                standing == "Academic Suspension");
      });
      if (at_risk.empty())
        std::cout << Color::GREEN << "  None – all students in good standing!\n"
                  << Color::RESET;
      else
        for (auto *s : at_risk)
          s->printSummary();
      break;
    }
    case 0:
      running = false;
      break;
    }
    if (choice != 0)
      Utils::pauseScreen();
  }
}

static void featureExportCSV(StudentManager &mgr) {
  Utils::clearScreen();
  std::cout << Color::BOLD << Color::CYAN << "\n  ╔══ EXPORT CSV ══╗\n"
            << Color::RESET;
  std::string fname = Utils::getLine("  Output filename (e.g. report.csv): ");
  if (fname.find('.') == std::string::npos)
    fname += ".csv";
  try {
    mgr.exportCSV(fname);
    std::cout << Color::GREEN << "  ✔ Exported to '" << fname << "'\n"
              << Color::RESET;
  } catch (const std::exception &e) {
    std::cout << Color::RED << "  ✘ " << e.what() << "\n" << Color::RESET;
  }
  Utils::pauseScreen();
}

static void seedDemoData(StudentManager &mgr) {
  struct DemoStudent {
    const char *name;
    int age;
    const char *gender;
    const char *email;
    const char *phone;
    const char *dept;
    int sem;
    const char *enroll;
  };
  std::vector<DemoStudent> demos = {
      {"Alice Johnson", 21, "Female", "alice.j@university.edu", "555-0101",
       "Computer Science", 3, "2023-09-01"},
      {"Bob Martinez", 22, "Male", "bob.m@university.edu", "555-0102",
       "Electronics", 4, "2023-01-15"},
      {"Chloe Kim", 20, "Female", "chloe.k@university.edu", "555-0103",
       "Mathematics", 2, "2024-01-10"},
      {"David Osei", 23, "Male", "david.o@university.edu", "555-0104",
       "Computer Science", 5, "2022-09-01"},
      {"Eva Schmidt", 21, "Female", "eva.s@university.edu", "555-0105",
       "Physics", 3, "2023-09-01"},
      {"Frank Lee", 24, "Male", "frank.l@university.edu", "555-0106",
       "Mechanical Eng", 6, "2022-01-10"},
      {"Grace Patel", 20, "Female", "grace.p@university.edu", "555-0107",
       "Computer Science", 2, "2024-09-01"},
      {"Hiro Tanaka", 22, "Male", "hiro.t@university.edu", "555-0108",
       "Electronics", 4, "2023-01-10"},
      {"Isla Brown", 21, "Female", "isla.b@university.edu", "555-0109",
       "Mathematics", 3, "2023-09-01"},
      {"James Wilson", 23, "Male", "james.w@university.edu", "555-0110",
       "Physics", 5, "2022-09-01"},
  };

  std::vector<std::vector<Course>> courses = {
      {{"CS101", "Intro to Programming", 92, 3},
       {"CS201", "Data Structures", 88, 3},
       {"MATH101", "Calculus I", 79, 3}},
      {{"EE101", "Circuit Analysis", 75, 3},
       {"EE201", "Digital Electronics", 80, 3},
       {"MATH101", "Calculus I", 68, 3}},
      {{"MATH101", "Calculus I", 95, 3},
       {"MATH201", "Linear Algebra", 91, 3},
       {"STAT101", "Statistics", 85, 3}},
      {{"CS301", "Algorithms", 84, 3},
       {"CS401", "OS Concepts", 78, 3},
       {"CS201", "Data Structures", 90, 3}},
      {{"PHY101", "Mechanics", 72, 3},
       {"PHY201", "Electromagnetism", 68, 3},
       {"MATH101", "Calculus I", 76, 3}},
      {{"ME101", "Statics", 55, 3},
       {"ME201", "Thermodynamics", 48, 3},
       {"MATH201", "Linear Algebra", 52, 3}},
      {{"CS101", "Intro to Programming", 97, 3},
       {"CS201", "Data Structures", 94, 3},
       {"MATH101", "Calculus I", 90, 3}},
      {{"EE301", "Microprocessors", 63, 3},
       {"EE201", "Digital Electronics", 70, 3},
       {"MATH201", "Linear Algebra", 59, 3}},
      {{"MATH301", "Real Analysis", 88, 3},
       {"STAT201", "Probability", 82, 3},
       {"MATH101", "Calculus I", 91, 3}},
      {{"PHY301", "Quantum Mechanics", 41, 3},
       {"PHY201", "Electromagnetism", 38, 3},
       {"MATH201", "Linear Algebra", 45, 3}},
  };

  for (size_t i = 0; i < demos.size(); ++i) {
    const auto &d = demos[i];
    Student s(0, d.name, d.age, d.gender, d.email, d.phone, d.dept, d.sem,
              d.enroll);
    int newId = mgr.addStudent(s);
    Student *sp = mgr.findById(newId);
    for (const auto &c : courses[i])
      sp->addCourse(c);
  }
  mgr.saveToFile(DATA_FILE);
  std::cout << Color::GREEN << "  ✔ Demo data loaded (" << demos.size()
            << " students).\n"
            << Color::RESET;
}

static void enableAnsiWindows() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  if (GetConsoleMode(hOut, &dwMode)) {
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  }
#endif
}

int main() {
  enableAnsiWindows();

  StudentManager mgr;

  int loaded = mgr.loadFromFile(DATA_FILE);
  if (loaded == 0) {
    Utils::clearScreen();
    printBanner();
    std::cout << Color::YELLOW
              << "  No existing data found. Loading demo students...\n"
              << Color::RESET;
    seedDemoData(mgr);
    Utils::pauseScreen();
  }

  bool running = true;
  while (running) {
    Utils::clearScreen();
    printBanner();

    std::cout << Color::DIM << "  Active Students: " << mgr.count(true)
              << "  |  Total Records: " << mgr.count(false)
              << "  |  Data file: " << DATA_FILE << "\n"
              << Color::RESET;

    printMainMenu();
    int choice = Utils::getInt("\n  Choose an option: ", 0, 7);

    switch (choice) {
    case 1:
      featureAddStudent(mgr);
      break;
    case 2:
      featureViewSearch(mgr);
      break;
    case 3:
      featureEditStudent(mgr);
      break;
    case 4:
      featureManageCourses(mgr);
      break;
    case 5:
      featureDeleteRestore(mgr);
      break;
    case 6:
      featureReports(mgr);
      break;
    case 7:
      featureExportCSV(mgr);
      break;
    case 0:
      if (Utils::getYesNo("  Save and exit?")) {
        mgr.saveToFile(DATA_FILE);
        std::cout << Color::GREEN << "\n  ✔ Data saved. Goodbye!\n"
                  << Color::RESET;
        running = false;
      }
      break;
    }
  }
  return 0;
}
