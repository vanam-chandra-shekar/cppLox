#include "Lox.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "AstPrinter.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"


bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
Interpreter Lox::interpreter;

void  Lox::main(int argL , char** arg)
{
    if(argL > 2)
    {
        std::cout<<"Usage: main [script] \n";
    }
    else if(argL == 2)
    {
        runFile(arg[1]);
    }
    else {
        runPromt();
    }
}

void Lox::runFile(const std::string &path)
{
    std::ifstream file(path);

    if(!file.is_open())
    {
        //exit if unable to open
        std::cerr<<"Error: unable to open "+path+"\n";
        exit(EXIT_FAILURE);
    }

    std::string str;
    file.seekg(0 , std::ios::end); //seting to end;
    auto sz = file.tellg();
    str.resize(sz); //reserving memory for string tellg tells size of a stream from start to current
    file.seekg(0 , std::ios::beg); //seting to end;
    file.read(&str[0], sz);

    run(str);

    if(hadError) exit(65);
    if(hadRuntimeError) exit(70);
}

void Lox::runPromt()
{
    std::string line;
    for(;;)
    {
        std::cout<<"> ";
        if(!std::getline(std::cin , line)) break;
        run(line);
        hadError = false;
    }
}

void Lox::run(const std::string& src)
{
    Scanner scanner(src);
    std::vector<Token>& tokens = scanner.scanTokens();

    Parser parser(tokens);
    
    std::shared_ptr<Expr> expr = parser.parse();

    if(hadError) return;

    // std::cout<<AstPrinter{}.print(expr)<<"\n";
        
   interpreter.interpret(expr);


}

void Lox::error(int line, const std::string_view &message)
{
    report(line, "", message);
}

    
void Lox::error(Token token , const std::string_view& message)
{
    if(token.type == TEOF)
    {
        report(token.line , " at end " , message);
    }
    else
    {
        report(token.line , " at '"+token.lexeme+"'" , message);
    }
}

void Lox::report(int line , const std::string_view& where , const std::string_view& message)
{
    std::cerr<<"[Line "<<line<<"] Error "<<where <<": "<<message<<"\n";
    hadError = false;
}

void Lox::runTimeError(const RuntimeError& error)
{
    std::cerr<<error.what() <<"[line :" << error.token.line << "]";
    hadRuntimeError = true;
}