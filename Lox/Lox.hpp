#pragma  once

#include <string>
#include "Interpreter.hpp"
#include "Token.hpp"

#include "RuntimeError.hpp"

class Lox
{

public:

    static bool hadError;
    static bool hadRuntimeError;

    static void main(int argv , char** argc);
    static void runFile(const std::string& path);
    static void runPromt();
    static void run(const std::string& src);
    static void error(int line , const std::string_view& message);
    static void error(Token token , const std::string_view& message);
    static void runTimeError(const RuntimeError& err);

private:
    static void report(int line , const std::string_view& where , const std::string_view& message );

    static Interpreter interpreter;
};

