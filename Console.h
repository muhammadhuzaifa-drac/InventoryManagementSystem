#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Console {
    // Colors
    enum Color {
        RESET = 0,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        BOLD_RED,
        BOLD_GREEN,
        BOLD_YELLOW,
        BOLD_BLUE,
        BOLD_CYAN
    };

    inline void setColor(Color color) {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int attr = 7; // default white
        switch (color) {
            case RESET: attr = 7; break; // default normal white
            case RED: attr = 4; break;
            case GREEN: attr = 2; break;
            case YELLOW: attr = 6; break;
            case BLUE: attr = 1; break;
            case MAGENTA: attr = 5; break;
            case CYAN: attr = 3; break;
            case WHITE: attr = 7; break;
            case BOLD_RED: attr = 12; break;
            case BOLD_GREEN: attr = 10; break;
            case BOLD_YELLOW: attr = 14; break;
            case BOLD_BLUE: attr = 9; break;
            case BOLD_CYAN: attr = 11; break;
            default: attr = 7; break;
        }
        SetConsoleTextAttribute(hConsole, attr);
#else
        // ANSI escape codes for Linux/macOS
        switch (color) {
            case RESET: std::cout << "\033[0m"; break;
            case RED: std::cout << "\033[31m"; break;
            case GREEN: std::cout << "\033[32m"; break;
            case YELLOW: std::cout << "\033[33m"; break;
            case BLUE: std::cout << "\033[34m"; break;
            case MAGENTA: std::cout << "\033[35m"; break;
            case CYAN: std::cout << "\033[36m"; break;
            case WHITE: std::cout << "\033[37m"; break;
            case BOLD_RED: std::cout << "\033[1;31m"; break;
            case BOLD_GREEN: std::cout << "\033[1;32m"; break;
            case BOLD_YELLOW: std::cout << "\033[1;33m"; break;
            case BOLD_BLUE: std::cout << "\033[1;34m"; break;
            case BOLD_CYAN: std::cout << "\033[1;36m"; break;
        }
#endif
    }

    inline void clear() {
#ifdef _WIN32
        // Cross-platform CLI clear
        system("cls");
#else
        system("clear");
#endif
    }

    inline void printHeader(const std::string& title) {
        setColor(BOLD_CYAN);
        std::cout << "\n======================================================================\n";
        std::cout << "  " << title << "\n";
        std::cout << "======================================================================\n";
        setColor(RESET);
    }

    inline void printDivider() {
        setColor(CYAN);
        std::cout << "----------------------------------------------------------------------\n";
        setColor(RESET);
    }

    // Helper for robust integer input
    inline int readInt(const std::string& prompt, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) {
        int val;
        while (true) {
            setColor(WHITE);
            std::cout << prompt;
            setColor(BOLD_CYAN);
            if (std::cin >> val) {
                if (val >= minVal && val <= maxVal) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    setColor(RESET);
                    return val;
                }
                setColor(BOLD_RED);
                std::cout << "Error: Value must be between " << minVal << " and " << maxVal << ".\n";
            } else {
                setColor(BOLD_RED);
                std::cout << "Error: Invalid input. Please enter an integer.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    // Helper for robust double input
    inline double readDouble(const std::string& prompt, double minVal = 0.0, double maxVal = std::numeric_limits<double>::max()) {
        double val;
        while (true) {
            setColor(WHITE);
            std::cout << prompt;
            setColor(BOLD_CYAN);
            if (std::cin >> val) {
                if (val >= minVal && val <= maxVal) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    setColor(RESET);
                    return val;
                }
                setColor(BOLD_RED);
                std::cout << "Error: Value must be between " << minVal << " and " << maxVal << ".\n";
            } else {
                setColor(BOLD_RED);
                std::cout << "Error: Invalid input. Please enter a decimal number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    // Helper for robust string input
    inline std::string readString(const std::string& prompt, bool allowEmpty = false) {
        std::string val;
        while (true) {
            setColor(WHITE);
            std::cout << prompt;
            setColor(BOLD_CYAN);
            std::getline(std::cin, val);
            setColor(RESET);
            if (!allowEmpty && val.empty()) {
                setColor(BOLD_RED);
                std::cout << "Error: Input cannot be empty.\n";
                continue;
            }
            return val;
        }
    }

    inline void pressEnterToContinue() {
        setColor(WHITE);
        std::cout << "\nPress Enter to continue...";
        setColor(RESET);
        std::string dummy;
        std::getline(std::cin, dummy);
    }
}

#endif // CONSOLE_H
