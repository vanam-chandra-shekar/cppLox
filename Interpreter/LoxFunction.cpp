#include "LoxFunction.hpp"

#include "Interpreter.hpp"
#include "Environment.hpp"
#include "LoxReturn.hpp"

LoxFunction::LoxFunction(std::shared_ptr<Stmt::Function> _decl ,std::shared_ptr<Environment> _closure) 
    : declaration{std::move(_decl)} ,closure{std::move(_closure)}
{}

int LoxFunction::arity()
{
    return declaration->params.size();
}

std::any LoxFunction::call(Interpreter &interepreter, std::vector<std::any> arguments)
{
    auto environment = std::make_shared<Environment>(closure);

    for(int i=0;i< declaration->params.size() ; i++)
    {
        environment->define(declaration->params.at(i).lexeme, arguments.at(i));
    }

    try
    {
        interepreter.executeBlock(declaration->body , environment);
    }
    catch(Return returnValue)
    {
        return returnValue.value;
    }
    

    return nullptr;
}

std::string LoxFunction::toString()
{
    return "<fn " + declaration->name.lexeme + ">";
}
