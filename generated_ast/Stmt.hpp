#pragma once
#include "Expr.hpp"
#include <memory>
#include <any>
#include "Token.hpp"

class Stmt {
public:
	class Visitor;
	class Expression;
	class Print;
	class Var;

	virtual std::any accept(Visitor& visitor) = 0;
	virtual ~Stmt() = default;
};

class Stmt::Visitor{
public:
	virtual std::any visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) = 0;
	virtual std::any visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) = 0;
	virtual std::any visitVarStmt(std::shared_ptr<Stmt::Var> stmt) = 0;

	virtual ~Visitor() = default;
};

class Stmt::Expression : public Stmt , public std::enable_shared_from_this<Expression> {
public:
	std::shared_ptr<Expr> expression;

	Expression(std::shared_ptr<Expr> _expression)
		: expression(std::move(_expression)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitExpressionStmt(shared_from_this());
	}
};

class Stmt::Print : public Stmt , public std::enable_shared_from_this<Print> {
public:
	std::shared_ptr<Expr> expression;

	Print(std::shared_ptr<Expr> _expression)
		: expression(std::move(_expression)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitPrintStmt(shared_from_this());
	}
};

class Stmt::Var : public Stmt , public std::enable_shared_from_this<Var> {
public:
	Token name;
	std::shared_ptr<Expr> initializer;

	Var(Token _name, std::shared_ptr<Expr> _initializer)
		: name(std::move(_name)), initializer(std::move(_initializer)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitVarStmt(shared_from_this());
	}
};

