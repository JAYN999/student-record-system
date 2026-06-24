#pragma once
#include <string>
#include <stdexcept>

namespace Utils {
    void clearScreen();
    void pauseScreen();

    std::string getLine(const std::string& prompt, bool required = true);
    int         getInt (const std::string& prompt, int minVal, int maxVal);
    double      getDouble(const std::string& prompt, double minVal, double maxVal);
    char        getChar(const std::string& prompt, const std::string& allowed);
    bool        getYesNo(const std::string& prompt);

    bool isValidEmail(const std::string& email);
    bool isValidDate (const std::string& date);
    bool isValidPhone(const std::string& phone);

    std::string trim   (const std::string& s);

    std::string currentDate();
} // namespace Utils
