#pragma once
#include "TokenType.hpp"
#include <ostream>
#include <type_traits>
#include <variant>
#include <string>

typedef std::variant<double , bool> Object;

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
    inline std::string objToString() const
    {
        std::string literalStr = std::visit([](auto&& arg) -> std::string{
            using T = std::decay_t<decltype(arg)>;

            if constexpr ( std::is_same_v<T, bool> )
            {
                return ((arg)? "true" : "false");
            }
            else
            {
                return std::to_string(arg);
            }
        },literal);

        return literalStr;
    }
};

inline std::ostream& operator<<(std::ostream& out , const Token& token)
{
    out<<token.type<<" "<<token.lexeme<<" "<< token.objToString()<<"\n";
    return out;
}

