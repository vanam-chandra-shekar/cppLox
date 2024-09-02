#pragma  once

#include <string>

class Lox
{

public:
    static void main(int argv , char** argc);
    static void runFile(const std::string& path);
    static void runPromt();
    static void run(const std::string& src);
};

