#pragma once
#include <string>

class ConsoleInput {
public:
    static int readInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static std::string readString(const std::string& prompt);        
    static std::string readOptionalString(const std::string& prompt); 
    static std::string readDate(const std::string& prompt);          

private:
    static void clearInput();
};