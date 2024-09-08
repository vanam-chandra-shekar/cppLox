#pragma once
#include "TokenType.hpp"
// #include <cstddef>
#include <ostream>
// #include <variant>
#include <string>
#include <any>

// A type used to store amany types it is a type safe union
// typedef std::variant<std::nullptr_t , double , bool , std::string > Object;
// typedef std::any Object;

class Token
{
public:
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const int line;

public:
    Token(const TokenType& _type , const std::string& _lexeme ,const std::any& _literal , const int& _line )
        : type(_type) , lexeme(_lexeme) , literal(_literal) ,line(_line)
    {}

private:
    friend std::ostream& operator<<(std::ostream& out , const Token& token);

    // converts a Objecct aka std::variant<std::nullptr_t , double , bool , std::string> to a String
    inline std::string objToString() const
    {

        if(literal.type() == typeid(bool))        
        {
            return ((std::any_cast<bool>(literal))? "true" : "false");
        }

       if(literal.type() == typeid(double))
       {
            return std::to_string(std::any_cast<double>(literal));
       }

       if(literal.type() == typeid(std::nullptr_t))
       {
            return "nil";
       }

       if(literal.type() == typeid(std::string))
       {
            return std::any_cast<std::string>(literal);
       }
    
    }
};

// operatorOverload of << operator for Token class
inline std::ostream& operator<<(std::ostream& out , const Token& token)
{
    out<<token.type<<" "<<token.lexeme<<" "<< token.objToString()<<"\n";
    return out;
}

