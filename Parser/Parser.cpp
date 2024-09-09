#include "Parser.hpp"
#include "Stmt.hpp"
#include <memory>
// #include <cassert>


std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
    std::vector<std::shared_ptr<Stmt>> statements;
    while(!isAtEnd())
    {
        statements.push_back(declaration());
    }

    return statements;
    
}

std::shared_ptr<Stmt> Parser::declaration()
{
    try
    {
        if(match(TVAR)) return valDeclaration();

        return statement();
    }
    catch(ParseError err)
    {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::statement()
{
    if(match(TPRINT)) return printStatement();

    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::valDeclaration()
{
    Token name = consume(TIDENTIFIER,"Expect variable name.");

    std::shared_ptr<Expr> init = nullptr;

    if(match(TEQUAL))
    {
        init = expression();
    }

    consume(TSEMICOLON,"Expect ';' after variable declaration.");
    return std::make_shared<Stmt::Var>(name , init);

}

std::shared_ptr<Stmt> Parser::printStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(TSEMICOLON , "Expect ';' after value.");

    return std::make_shared<Stmt::Print>(value);
}

std::shared_ptr<Stmt> Parser::expressionStatement()
{
    std::shared_ptr<Expr> expr = expression();
    consume(TSEMICOLON,"Expect ';' after Expression.");

    return std::make_shared<Stmt::Expression>(expr);
}



std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparison();

    while (match(TBANG_EQUAL , TEQUAL_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();

        expr = std::make_shared<Expr::Binary> (expr , std::move(op) , right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    std::shared_ptr<Expr> expr = term();

    while(match(TGREATER , TGREATER_EQUAL , TLESS , TLESS_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Expr::Binary> (expr , std::move(op) , right);
    }
    return expr;
}

// term           → factor ( ( "-" | "+" ) factor )* ;
// factor         → unary ( ( "/" | "*" ) unary )* ;

std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();

    while(match(TMINUS , TPLUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Expr::Binary> (expr , std::move(op) , right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = unary();

    while(match(TSLASH , TSTAR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Expr::Binary> (expr , std::move(op) , right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if(match(TBANG , TMINUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Expr::Unary>(std::move(op) , right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
    if(match(TFALSE)) return std::make_shared<Expr::Literal>(false);
    if(match(TTRUE))  return std::make_shared<Expr::Literal>(true);

    if(match(TNIL)) return std::make_shared<Expr::Literal>(nullptr);

    if(match(TNUMBER , TSTRING))
    {
        return  std::make_shared<Expr::Literal>(previous().literal);
    }

    if(match(TLEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = expression();
        consume(TRIGHT_PAREN , "Expected ')' after Expression");
        return std::make_shared<Expr::Grouping>(expr);
    }

    if(match(TIDENTIFIER))
    {
        return std::make_shared<Expr::Variable>(previous());
    }

    throw error(peek() , "Expected expression.");
}

