#include "ConsoleInput.h"
#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>

void ConsoleInput::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int ConsoleInput::readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof()) throw std::runtime_error("Ввод завершён");
        clearInput();
        std::cout << "Ошибка: введите целое число.\n";
    }
}

double ConsoleInput::readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof()) throw std::runtime_error("Ввод завершён");
        clearInput();
        std::cout << "Ошибка: введите число (например, 150.50).\n";
    }
}

std::string ConsoleInput::readString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    if (!std::getline(std::cin >> std::ws, value)) throw std::runtime_error("Ввод завершён");
    return value;
}

std::string ConsoleInput::readOptionalString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    if (!std::getline(std::cin, value)) throw std::runtime_error("Ввод завершён");
    return value;
}

std::string ConsoleInput::readDate(const std::string& prompt) {
    static const std::regex date_format(R"(\d{4}-\d{2}-\d{2})");
    while (true) {
        std::string value = readString(prompt);
        if (std::regex_match(value, date_format)) return value;
        std::cout << "Ошибка: дата должна быть в формате ГГГГ-ММ-ДД (например, 2026-11-25).\n";
    }
}