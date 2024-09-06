#include <memory>
#include <any>
#include "Token.hpp"

class Expr {
public:
	class Visitor;
	class Binary;
	class Grouping;
	class Literal;
	class Unary;

	virtual std::any accept(Visitor& visitor) = 0;
	virtual ~Expr() = default;
};

class Expr::Visitor{
public:
	virtual std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) = 0;
	virtual std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) = 0;
	virtual std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) = 0;
	virtual std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) = 0;

	virtual ~Visitor() = default;
};

class Expr::Binary : Expr , public std::enable_shared_from_this<Binary> {
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

class Expr::Grouping : Expr , public std::enable_shared_from_this<Grouping> {
public:
	std::shared_ptr<Expr> expression;

	Grouping(std::shared_ptr<Expr> _expression)
		: expression(std::move(_expression)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitGroupingExpr(shared_from_this());
	}
};

class Expr::Literal : Expr , public std::enable_shared_from_this<Literal> {
public:
	Object value;

	Literal(Object _value)
		: value(std::move(_value)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitLiteralExpr(shared_from_this());
	}
};

class Expr::Unary : Expr , public std::enable_shared_from_this<Unary> {
public:
	Token op;
	std::shared_ptr<Expr> right;

	Unary(Token _op, std::shared_ptr<Expr> _right)
		: op(std::move(_op)), right(std::move(_right)) {}

	inline std::any accept(Visitor& visitor) override {
		return visitor.visitUnaryExpr(shared_from_this());
	}
};
