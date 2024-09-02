#include "Lox.hpp"
#include <fstream>
#include <iostream>
#include <string>



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
    }
}

void Lox::run(const std::string& src)
{
    std::cout<<src<<"\n";
}