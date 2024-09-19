#pragma once

#include <chrono>
#include <vector>

#include "Stmt.hpp"
#include "Expr.hpp"
#include "Environment.hpp"
#include "LoxCallable.hpp"


class nativeClock : public LoxCallable
{
    int arity() override {return 0;}
    std::any call(Interpreter& interepreter , std::vector<std::any> arguments ) override
    {
        auto ticks = std::chrono::system_clock::now().time_since_epoch();

        return std::chrono::duration_cast<std::chrono::milliseconds>(ticks);
    }

    std::string toString() override
    {
        return "<native fn>";
    }
};


class Interpreter : public Expr::Visitor , public Stmt::Visitor
{

public:

    Interpreter()
    {
        global->define("Clock", std::shared_ptr<nativeClock>{});
    }


    std::any visitAssignExpr(std::shared_ptr<Expr::Assign> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) override;
    std::any visitVariableExpr(std::shared_ptr<Expr::Variable> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Expr::Logical> expr) override;
    std::any visitCallExpr(std::shared_ptr<Expr::Call> expr) override;

    std::any visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Stmt::Var> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) override;

    //control flow implementation
    std::any visitIfStmt(std::shared_ptr<Stmt::If> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<Stmt::While> stmt) override;

    std::any visitFunctionStmt(std::shared_ptr<Stmt::Function> stmt) override;


    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);





private:

    std::shared_ptr<Environment> global {new Environment};
    std::shared_ptr<Environment> environment = global;

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