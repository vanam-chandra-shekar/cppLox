#pragma  once

#include <string>
#include <Token.hpp>

class Lox
{

public:

    static bool hadError;

    static void main(int argv , char** argc);
    static void runFile(const std::string& path);
    static void runPromt();
    static void run(const std::string& src);
    static void error(int line , const std::string_view& message);
    static void error(Token token , const std::string_view& message);

private:
    static void report(int line , const std::string_view& where , const std::string_view& message );

};

