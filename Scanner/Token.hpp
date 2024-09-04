#pragma once
#include "TokenType.hpp"
#include <cstddef>
#include <ostream>
#include <type_traits>
#include <variant>
#include <string>

// A type used to store amany types it is a type safe union
typedef std::variant<std::nullptr_t , double , bool , std::string > Object;

class Token
{

    const TokenType type;
    const std::string lexeme;
    const Object literal;
    const int line;

public:
    Token(const TokenType& _type , const std::string& _lexeme ,const Object& _literal , const int& _line )
        : type(_type) , lexeme(_lexeme) , literal(_literal) ,line(_line)
    {}

private:
    friend std::ostream& operator<<(std::ostream& out , const Token& token);

    // converts a Objecct aka std::variant<std::nullptr_t , double , bool , std::string> to a String
    inline std::string objToString() const
    {
        std::string literalStr = std::visit([](auto&& arg) -> std::string{
            //Gets the Dacayed type of args
            //std::decay_t converts decayed type to orginal type
            using T = std::decay_t<decltype(arg)>;

            // std::is_same_v checks if two types are same
            if constexpr ( std::is_same_v<T, bool> )
            {
                return ((arg)? "true" : "false");
            }
            else if constexpr ( std::is_same_v<T , std::nullptr_t>)
            {
                return "null";
            }
            else if constexpr ( std::is_same_v<T , std::string>)
            {
                return arg;
            }
            else {
                return std::to_string(arg);
            }
        },literal);

        return literalStr;
    }
};

// operatorOverload of << operator for Token class
inline std::ostream& operator<<(std::ostream& out , const Token& token)
{
    out<<token.type<<" "<<token.lexeme<<" "<< token.objToString()<<"\n";
    return out;
}

