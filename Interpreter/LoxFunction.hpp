#pragma once

#include "LoxCallable.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"
#include <memory>
class LoxFunction : public LoxCallable
{

private:
    std::shared_ptr<Stmt::Function> declaration;
    std::shared_ptr<Environment> closure;

public:
    LoxFunction( std::shared_ptr<Stmt::Function> _decl , std::shared_ptr<Environment> closure);

    int arity() override;
    std::any call(Interpreter& interepreter , std::vector<std::any> arguments ) override;
    std::string toString() override;

};