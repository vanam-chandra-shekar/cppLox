#include "Parser.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "TokenType.hpp"
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

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
    std::vector<std::shared_ptr<Stmt>> statements;

    while(!check(TRIGHT_BRACE) && !isAtEnd())
    {
        statements.emplace_back(declaration());
    }

    consume(TRIGHT_BRACE , "Expect  '}' after block.");
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
    if(match(TLEFT_BRACE)) return std::make_shared<Stmt::Block>(block());
    if(match(TIF)) return ifStatement();
    if(match(TWHILE)) return whileStatement();
    if(match(TFOR)) return forStatement();

    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::forStatement()
{
    consume(TLEFT_PAREN , "Expect '(' after for.");
    std::shared_ptr<Stmt> init;
    if(match(TSEMICOLON))
    {
        init = nullptr;
    } 
    else if(match(TVAR))
    {
        init = valDeclaration();
    }
    else
    {
        init = expressionStatement();
    }

    std::shared_ptr<Expr> cond = nullptr;
    if(!check(TSEMICOLON))
    {
        cond = expression();
    }
    consume(TSEMICOLON , "Expect ';' after loop condition.");

    std::shared_ptr<Expr> incre = nullptr;
    if(!check(TRIGHT_PAREN))
    {
        incre = expression();
    }
    consume(TRIGHT_PAREN , "Expect ')' after for clauses.");

    std::shared_ptr<Stmt> body = statement();

    if(incre != nullptr)
    {
        std::vector<std::shared_ptr<Stmt>> statements;
        statements.push_back(body);
        statements.emplace_back(std::make_shared<Stmt::Expression>(incre));

        body = std::make_shared<Stmt::Block>(std::move(statements));
    }

    if(cond == nullptr)
    {
        cond = std::make_shared<Expr::Literal>(true);
    }

    body = std::make_shared<Stmt::While>(cond , body);

    if(init != nullptr)
    {
        std::vector<std::shared_ptr<Stmt>> statements;
        statements.push_back(init);
        statements.push_back(body);

        body = std::make_shared<Stmt::Block>(statements);
    }    

    return body;
}

std::shared_ptr<Stmt> Parser::whileStatement()
{
    consume(TLEFT_PAREN, "Expect '(' after while.");
    std::shared_ptr<Expr> cond = expression();
    consume(TRIGHT_PAREN,"Expect ')' after while condition.");

    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<Stmt::While>(cond , body);

}

std::shared_ptr<Stmt> Parser::ifStatement()
{
    consume(TLEFT_PAREN , "Expect '(' after if.");
    std::shared_ptr<Expr> cond = expression();
    consume(TRIGHT_PAREN , "Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch = statement();

    std::shared_ptr<Stmt> elseBranch = nullptr;

    if(match(TELSE)) 
    {
        elseBranch = statement();
    }

    return std::make_shared<Stmt::If>(cond , thenBranch , elseBranch); 

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
    return std::make_shared<Stmt::Var>(std::move(name) , init);

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
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
    std::shared_ptr<Expr> expr = logicalOr();

    if(match(TEQUAL))
    {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if(Expr::Variable* e = dynamic_cast<Expr::Variable*>(expr.get()))
        {
            Token name = e->name;;
            return std::make_shared<Expr::Assign>(name , value);
        }

        error(std::move(equals), "Invalid assignment target.");
    }

    return expr;

}

std::shared_ptr<Expr> Parser::logicalOr()
{
    std::shared_ptr<Expr> expr = logicalAnd();
    
    while(match(TOR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Expr::Logical>(expr , op , right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logicalAnd()
{
    std::shared_ptr<Expr> expr = equality();
    
    while(match(TAND))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Expr::Logical>(expr , op , right);
    }
    return expr;
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

