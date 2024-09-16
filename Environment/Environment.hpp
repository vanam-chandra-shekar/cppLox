#pragma once
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include "RuntimeError.hpp"

class Environment : public std::enable_shared_from_this<Environment>
{

private:
    std::unordered_map<std::string , std::any> values;

    std::shared_ptr<Environment> enclosing;


public:

    Environment()
        : enclosing(nullptr){}
    
    Environment(std::shared_ptr<Environment> _enclosing)
        : enclosing(std::move(_enclosing)) {}

    inline void define(const std::string& name , std::any value)
    {
        values[name] = std::move(value);
    }

    inline std::any get(const Token& name)
    {
        auto elem = values.find(name.lexeme);
        if(elem != values.end())
        {
            return elem->second;
        }

        if(enclosing != nullptr) return enclosing->get(name);

        throw RuntimeError(name ,"Undefined variable '"+name.lexeme+"'." );
    }

    inline void assign(Token name , std::any value)
    {
        auto elm = values.find(name.lexeme);

        if(elm != values.end())
        {
            elm->second = std::move(value);
            return;
        }

        if(enclosing != nullptr)
        {
            enclosing->assign(name, std::move(value));
            return;
        }

        throw RuntimeError(name , "Undefined variable '"+ name.lexeme+"'.");

    }

};