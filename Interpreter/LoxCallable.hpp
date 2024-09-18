#pragma once


#include "Interpreter.hpp"
#include <any>
class LoxCallable
{
public:
    virtual int arity() = 0;
    virtual std::any call(Interpreter& interepreter , std::vector<std::any> arguments ) = 0;
    virtual ~LoxCallable() = default;
};