#pragma once
#include "Expr.hpp"
#include <vector>
#include <memory>
#include <any>
#include "Token.hpp"

class Stmt {
public:
	class Visitor;
	class Expression;
	class Print;
	class Var;
	class Block;
	class If;
	class While;
	class Function;

	virtual std::any accept(Visitor& visitor) = 0;
	virtual ~Stmt() = default;
};

class Stmt::Visitor{
public:
	virtual std::any visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) = 0;
	virtual std::any visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) = 0;
	virtual std::any visitVarStmt(std::shared_ptr<Stmt::Var> stmt) = 0;
	virtual std::any visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) = 0;
	virtual std::any visitIfStmt(std::shared_ptr<Stmt::If> stmt) = 0;
	virtual std::any visitWhileStmt(std::shared_ptr<Stmt::While> stmt) = 0;
	virtual std::any visitFunctionStmt(std::shared_ptr<Stmt::Function> stmt) = 0;

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

class Stmt::Block : public Stmt , public std::enable_shared_from_this<Block> {
public:
	std::vector<std::shared_ptr<Stmt>> statements;

	Block(std::vector<std::shared_ptr<Stmt>> _statements)
		: statements(std::move(_statements)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitBlockStmt(shared_from_this());
	}
};

class Stmt::If : public Stmt , public std::enable_shared_from_this<If> {
public:
	std::shared_ptr<Expr> expr;
	std::shared_ptr<Stmt> thenBranch;
	std::shared_ptr<Stmt> elseBranch;

	If(std::shared_ptr<Expr> _expr, std::shared_ptr<Stmt> _thenBranch, std::shared_ptr<Stmt> _elseBranch)
		: expr(std::move(_expr)), thenBranch(std::move(_thenBranch)), elseBranch(std::move(_elseBranch)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitIfStmt(shared_from_this());
	}
};

class Stmt::While : public Stmt , public std::enable_shared_from_this<While> {
public:
	std::shared_ptr<Expr> condition;
	std::shared_ptr<Stmt> body;

	While(std::shared_ptr<Expr> _condition, std::shared_ptr<Stmt> _body)
		: condition(std::move(_condition)), body(std::move(_body)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitWhileStmt(shared_from_this());
	}
};

class Stmt::Function : public Stmt , public std::enable_shared_from_this<Function> {
public:
	Token name;
	std::vector<Token> params;
	std::vector<std::shared_ptr<Stmt>> body;

	Function(Token _name, std::vector<Token> _params, std::vector<std::shared_ptr<Stmt>> _body)
		: name(std::move(_name)), params(std::move(_params)), body(std::move(_body)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitFunctionStmt(shared_from_this());
	}
};

