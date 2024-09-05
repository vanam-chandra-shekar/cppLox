#include "Token.hpp"

class Expr {
public:
	class Binary;
	class Grouping;
	class Literal;
	class Unary;
};

class Expr::Binary : public Expr {
public:
	Expr left;
	Token op;
	Expr right;

	Binary(Expr left , Token op , Expr right) :
		left(left), op(op), right(right) {}
};

class Expr::Grouping : public Expr {
public:
	Expr expression;

	Grouping(Expr expression) :
		expression(expression) {}
};

class Expr::Literal : public Expr {
public:
	Object value;

	Literal(Object value) :
		value(value) {}
};

class Expr::Unary : public Expr {
public:
	Token op;
	Expr right;

	Unary(Token op , Expr right) :
		op(op), right(right) {}
};

