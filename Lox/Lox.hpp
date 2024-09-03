#pragma  once

#include <string>

class Lox
{

public:

    static bool hadError;

    static void main(int argv , char** argc);
    static void runFile(const std::string& path);
    static void runPromt();
    static void run(const std::string& src);
    static void error(int line , const std::string& message);

private:
    static void report(int line , const std::string& where , const std::string& message );

};

