#pragma once

#include "Expr.hpp"


class Interpreter : public Expr::Visitor
{

public:
    std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) override;

    void interpret(std::shared_ptr<Expr> expr);



private:
    inline std::any evaluate(std::shared_ptr<Expr> expr)
    {
        return expr->accept(*this);
    }

    inline bool isTruthy(std::any& obj)
    {
        if(obj.type() == typeid(std::nullptr_t)) return false;
        if(obj.type() == typeid(bool)) return std::any_cast<bool>(obj);
        return true;
    }

};