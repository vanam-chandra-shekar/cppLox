#pragma once

#include "Token.hpp"
#include "TokenType.hpp"
#include <string>
#include <vector>


class Scanner
{
    const std::string source;
    std::vector<Token> tokens;

public:
    Scanner(const std::string& src)
        : source(src)
    {}

    std::vector<Token>& scanTokens();

private:

    int start = 0;
    int current = 0;
    int line = 1;


    bool isAtEnd();
    char advance();
    bool match(char expected); 
    char peek();
    char peekNext();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNum(char c);


    void string();
    void number();
    void identifier();

    void addToken(TokenType type , Object literal);
    void addToken(TokenType type);

    
    void scanToken();

};