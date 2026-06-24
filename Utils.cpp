#include "Utils.h"
#include "Common.h"
#include <limits>
#include <algorithm>
#include <regex>
#include <ctime>
#include <sstream>
#include <cctype>

#ifdef _WIN32
  #include <cstdlib>
#endif

namespace Utils {

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getLine(const std::string& prompt, bool required) {
    std::string input;
    while (true) {
        std::cout << "  " << prompt;
        std::getline(std::cin, input);
        input = trim(input);
        if (!required || !input.empty()) return input;
        std::cout << Color::RED << "  This field is required. Please try again." << Color::RESET << "\n";
    }
}

int getInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << "  " << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        try {
            size_t pos;
            int val = std::stoi(line, &pos);
            if (pos == line.size() && val >= minVal && val <= maxVal)
                return val;
        } catch (...) {}
        std::cout << Color::RED << "  Invalid input. Enter a number between "
                  << minVal << " and " << maxVal << "." << Color::RESET << "\n";
    }
}

double getDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        std::cout << "  " << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        try {
            size_t pos;
            double val = std::stod(line, &pos);
            if (pos == line.size() && val >= minVal && val <= maxVal)
                return val;
        } catch (...) {}
        std::cout << Color::RED << "  Invalid input. Enter a number between "
                  << minVal << " and " << maxVal << "." << Color::RESET << "\n";
    }
}

char getChar(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::cout << "  " << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        if (line.size() == 1) {
            char c = static_cast<char>(::tolower(static_cast<unsigned char>(line[0])));
            if (allowed.find(c) != std::string::npos) return c;
        }
        std::cout << Color::RED << "  Invalid choice. Valid options: [" << allowed << "]" << Color::RESET << "\n";
    }
}

bool getYesNo(const std::string& prompt) {
    char c = getChar(prompt + " [y/n]: ", "yn");
    return (c == 'y');
}

bool isValidEmail(const std::string& email) {
    const std::regex pattern(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}

bool isValidDate(const std::string& date) {
    const std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, pattern)) return false;
    int y = std::stoi(date.substr(0,4));
    int m = std::stoi(date.substr(5,2));
    int d = std::stoi(date.substr(8,2));
    if (m < 1 || m > 12 || d < 1 || d > 31) return false;
    if (y < 1900 || y > 2100) return false;
    return true;
}

bool isValidPhone(const std::string& phone) {
    const std::regex pattern(R"([\+\-\(\) 0-9]{7,15})");
    return std::regex_match(phone, pattern);
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end   = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string currentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* tm_info = std::localtime(&t);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info);
    return std::string(buf);
}

} // namespace Utils
