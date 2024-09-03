#pragma once

#include "Token.hpp"
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

};