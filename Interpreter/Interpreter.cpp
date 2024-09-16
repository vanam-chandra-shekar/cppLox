
#include <iostream>

#include "Lox.hpp"
#include "Interpreter.hpp"
#include "TokenType.hpp"




template <typename... Args>
bool checkIfDoubles(Token op,const Args&... args) {
    // Use initializer list to check each argument's type
    bool allDoubles = ((args.type() == typeid(double)) && ...);
    
    if(!allDoubles)
    {
        throw RuntimeError(op , "Operand must be Number.");
    }

    return allDoubles;
}

bool isEqual(const std::any& a, const std::any& b) {
    if(a.type() == typeid(std::nullptr_t) && b.type() == typeid(std::nullptr_t))  return true;

    if(a.type() == typeid(std::nullptr_t)) return false;
    if(a.type() == typeid(std::string) && b.type() == typeid(std::string))
    {
        return (std::any_cast<std::string>(a) == std::any_cast<std::string>(b));
    }

    if(a.type() == typeid(double) && b.type() == typeid(double))
    {
        return (std::any_cast<double>(a) == std::any_cast<double>(b));
    }

    if(a.type() == typeid(bool) && b.type() == typeid(bool))
    {
        return (std::any_cast<bool>(a) == std::any_cast<bool>(b));
    }

    return false;
}

std::string stringify(const std::any& object) {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(double)) {
      std::string text = std::to_string(
          std::any_cast<double>(object));
      if (text[text.length() - 2] == '.' &&
          text[text.length() - 1] == '0') {
        text = text.substr(0, text.length() - 2);
      }
      return text;
    }

    if (object.type() == typeid(std::string)) {
      return std::any_cast<std::string>(object);
    }
    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object) ? "true" : "false";
    }

    return "Error in stringify: object type not recognized.";
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements)
{
    try
    {
        for(auto& statement : statements)
        {
            execute(statement);
        }
    }
    catch(RuntimeError error)
    {
        Lox::runTimeError(error);
    }
}

void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements , std::shared_ptr<Environment> environment)
{
    std::shared_ptr<Environment> previous = this->environment;

    try
    {
        this->environment =  environment;

        for(const std::shared_ptr<Stmt>& statement : statements)
        {
            execute(statement);
        }
    }
    catch(...)
    {
        this->environment = previous;
        throw;
    }

    this->environment = previous;

}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Stmt::Block> stmt)
{
    executeBlock(stmt->statements , std::make_shared<Environment>(environment));
    return {};
}

std::any Interpreter::visitWhileStmt(std::shared_ptr<Stmt::While> stmt)
{
    std::any cond = evaluate(stmt->condition);

    while(isTruthy(cond))
    {
        execute(stmt->body);
        cond = evaluate(stmt->condition);
    }

    return {};

}


std::any Interpreter::visitIfStmt(std::shared_ptr<Stmt::If> stmt)
{
    std::any cond = evaluate(stmt->expr);
    if(isTruthy(cond))
    {
        execute(stmt->thenBranch);
    }
    else if(stmt->elseBranch != nullptr)
    {
        execute(stmt->elseBranch);
    }
    return {};
}


std::any Interpreter::visitVarStmt(std::shared_ptr<Stmt::Var> stmt)
{
    std::any value = nullptr;

    if(stmt->initializer != nullptr)
    {
        value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, value);
    return nullptr;
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Expr::Assign> expr)
{
    std::any value = evaluate(expr->value);
    environment->assign(expr->name , value);
    return value;
}


std::any Interpreter::visitVariableExpr(std::shared_ptr<Expr::Variable> expr)
{
    return environment->get(expr->name);
}


std::any Interpreter::visitPrintStmt(std::shared_ptr<Stmt::Print> stmt)
{
    std::any value = evaluate(stmt->expression);
    std::cout<<stringify(value)<<"\n";
    return {};
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt)
{
    evaluate(stmt->expression);
    return nullptr;
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr)
{
    return expr->value;
}


std::any Interpreter::visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr)
{
    return evaluate(expr->expression);
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Expr::Unary> expr)
{
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
        case TMINUS:
            return -1 * (std::any_cast<double>(right));
        case TBANG:
            return !isTruthy(right);
    }

    return {};

}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Expr::Logical> expr)
{
    std::any left = evaluate(expr->left);

    if(expr->op.type == TOR)
    {
        if(isTruthy(left))
        {
            return left;
        }
    }
    else
    {
        if(!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Expr::Binary> expr)
{
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch(expr->op.type)
    {
        case TGREATER:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) > std::any_cast<double>(right));
            }

        case TGREATER_EQUAL:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) >= std::any_cast<double>(right));
            }
        
        case TLESS:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) < std::any_cast<double>(right));
            }
        case TLESS_EQUAL:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) <= std::any_cast<double>(right));
            }

        case TMINUS:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) - std::any_cast<double>(right));
            }
        
        case TSLASH:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) / std::any_cast<double>(right));
            }
        
        case TSTAR:
            if(checkIfDoubles(expr->op,left , right))
            {
                return (std::any_cast<double>(left) * std::any_cast<double>(right));
            }
        
        case TBANG_EQUAL: return !isEqual(left , right);
        case TEQUAL_EQUAL: return isEqual(left , right);
        
        case TPLUS:
            if(left.type() == typeid(double) && right.type() == typeid(double))
            {
                return (std::any_cast<double>(left) + std::any_cast<double>(right));
            } 
            else if(left.type() == typeid(std::string) && right.type() == typeid(std::string))
            {
                return (std::any_cast<std::string>(left) + std::any_cast<std::string>(right));
            }

            throw RuntimeError(expr->op , "Operands must br two numbers or two strings.");
    }

    return nullptr;
}
