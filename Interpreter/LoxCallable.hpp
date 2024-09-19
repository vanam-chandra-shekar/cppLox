#pragma once

#include <any>
#include <vector>
#include <string>

class Interpreter;

class LoxCallable
{
public:
    virtual int arity() = 0;
    virtual std::any call(Interpreter& interepreter , std::vector<std::any> arguments ) = 0;
    virtual std::string toString() = 0;
    virtual ~LoxCallable() = default;
};