#include "Lox.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Scanner.hpp"


bool Lox::hadError = false;

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
}

void Lox::runPromt()
{
    std::string line;
    for(;;)
    {
        std::cout<<"> ";
        std::getline(std::cin , line);
        if(std::cin.eof()) break;  // ends if EOF is encountered
        run(line);
        hadError = false;
    }
}

void Lox::run(const std::string& src)
{
    Scanner scanner(src);
    std::vector<Token> tokens = scanner.scanTokens();


    for(Token token : tokens)
    {
        std::cout<<token<<"\n";
    }
}

void Lox::error(int line, const std::string &message)
{
    report(line, "", message);
}

void Lox::report(int line , const std::string& where , const std::string& message)
{
    std::cerr<<"[Line "<<line<<"] Error "+where +": "+message<<"\n";
    hadError = false;
}