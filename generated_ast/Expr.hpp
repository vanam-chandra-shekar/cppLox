#pragma once
#include <memory>
#include <any>
#include "Token.hpp"

class Expr {
public:
	class Visitor;
	class Assign;
	class Binary;
	class Grouping;
	class Literal;
	class Unary;
	class Variable;
	class Logical;

	virtual std::any accept(Visitor& visitor) = 0;
	virtual ~Expr() = default;
};

class Expr::Visitor{
public:
	virtual std::any visitAssignExpr(std::shared_ptr<Expr::Assign> expr) = 0;
	virtual std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) = 0;
	virtual std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) = 0;
	virtual std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) = 0;
	virtual std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) = 0;
	virtual std::any visitVariableExpr(std::shared_ptr<Expr::Variable> expr) = 0;
	virtual std::any visitLogicalExpr(std::shared_ptr<Expr::Logical> expr) = 0;

	virtual ~Visitor() = default;
};

class Expr::Assign : public Expr , public std::enable_shared_from_this<Assign> {
public:
	Token name;
	std::shared_ptr<Expr> value;

	Assign(Token _name, std::shared_ptr<Expr> _value)
		: name(std::move(_name)), value(std::move(_value)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitAssignExpr(shared_from_this());
	}
};

class Expr::Binary : public Expr , public std::enable_shared_from_this<Binary> {
public:
	std::shared_ptr<Expr> left;
	Token op;
	std::shared_ptr<Expr> right;

	Binary(std::shared_ptr<Expr> _left, Token _op, std::shared_ptr<Expr> _right)
		: left(std::move(_left)), op(std::move(_op)), right(std::move(_right)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitBinaryExpr(shared_from_this());
	}
};

class Expr::Grouping : public Expr , public std::enable_shared_from_this<Grouping> {
public:
	std::shared_ptr<Expr> expression;

	Grouping(std::shared_ptr<Expr> _expression)
		: expression(std::move(_expression)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitGroupingExpr(shared_from_this());
	}
};

class Expr::Literal : public Expr , public std::enable_shared_from_this<Literal> {
public:
	std::any value;

	Literal(std::any _value)
		: value(std::move(_value)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitLiteralExpr(shared_from_this());
	}
};

class Expr::Unary : public Expr , public std::enable_shared_from_this<Unary> {
public:
	Token op;
	std::shared_ptr<Expr> right;

	Unary(Token _op, std::shared_ptr<Expr> _right)
		: op(std::move(_op)), right(std::move(_right)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitUnaryExpr(shared_from_this());
	}
};

class Expr::Variable : public Expr , public std::enable_shared_from_this<Variable> {
public:
	Token name;

	Variable(Token _name)
		: name(std::move(_name)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitVariableExpr(shared_from_this());
	}
};

class Expr::Logical : public Expr , public std::enable_shared_from_this<Logical> {
public:
	std::shared_ptr<Expr> left;
	Token op;
	std::shared_ptr<Expr> right;

	Logical(std::shared_ptr<Expr> _left, Token _op, std::shared_ptr<Expr> _right)
		: left(std::move(_left)), op(std::move(_op)), right(std::move(_right)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitLogicalExpr(shared_from_this());
	}
};

