#pragma once

#include <vector>

#include "Stmt.hpp"
#include "Expr.hpp"
#include "Environment.hpp"


class Interpreter : public Expr::Visitor , public Stmt::Visitor
{

public:
    std::any visitAssignExpr(std::shared_ptr<Expr::Assign> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) override;
    std::any visitVariableExpr(std::shared_ptr<Expr::Variable> expr) override;

    std::any visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Stmt::Var> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) override;


    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);



private:

    std::shared_ptr<Environment> environment {new Environment};

    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements , std::shared_ptr<Environment> environment);

    inline std::any evaluate(std::shared_ptr<Expr> expr)
    {
        return expr->accept(*this);
    }

    inline void execute(std::shared_ptr<Stmt> stmt)
    {
        stmt->accept(*this);
    }

    inline bool isTruthy(std::any& obj)
    {
        if(obj.type() == typeid(std::nullptr_t)) return false;
        if(obj.type() == typeid(bool)) return std::any_cast<bool>(obj);
        return true;
    }

};