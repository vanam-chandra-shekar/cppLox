#pragma once

#include <sstream>
#include <string>
#include <assert.h>
#include "Expr.hpp"

class AstPrinter : public Expr::Visitor
{
public:
    inline std::string print(std::shared_ptr<Expr> expr)
    {
        return std::any_cast<std::string>(expr->accept(*this));
    }

    std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override
    {
        return parenthesize(expr->op.lexeme , expr->left , expr->right);
    }

    std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) override
    {
        return parenthesize("group" , expr->expression);
    }

    std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override
    {
        if(expr->value.type() == typeid(std::nullptr_t))
        {
            return "nil";
        }
        else if(expr->value.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(expr->value);
        }
        else if(expr->value.type() == typeid(double))
        {
            return std::to_string(std::any_cast<double>(expr->value));
        }
        else if(expr->value.type() == typeid(bool))
        {
            return (std::any_cast<bool>(expr->value) ? "true" : "false");
        }

        return "Error in visitLiteralExpr : literal type not recognized. ";

    }

    std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override
    {
        return parenthesize(expr->op.lexeme , expr->right);
    }

private:
    template <class ... E>
    std::string parenthesize(std::string name , E ... expr)
    {
        assert((... && std::is_same_v<E , std::shared_ptr<Expr>>));

        std::ostringstream builder;
        
        builder<<"("<<name;
        ((builder<<" "<<print(expr)) , ...);
        builder<<")";

        return builder.str();

    }

};