#pragma once

#include "Lox.hpp"
#include "Expr.hpp"
#include <vector>
#include <cassert>
#include "Stmt.hpp"



class Parser
{

private:

    struct ParseError : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    const std::vector<Token>& tokens;
    int current = 0;

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> logicalOr();
    std::shared_ptr<Expr> logicalAnd();

    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> statement();

    std::vector<std::shared_ptr<Stmt>> block();

    std::shared_ptr<Stmt> valDeclaration();

    //control flow
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::shared_ptr<Stmt> forStatement();



    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Stmt> expressionStatement();


    template <class ... T>
    inline bool match(T ... type)
    {
        assert((... && std::is_same_v<T , TokenType>));

        if( (... || check(type)))
        {
            advance();
            return true;
        }

        return false;
    }


    inline bool check(TokenType type)
    {
        if(isAtEnd()) return false;
        return peek().type == type;
    }

    inline Token advance()
    {
        if(!isAtEnd()) current++;
        return previous();
    }

    inline bool isAtEnd()
    {
        if(peek().type == TEOF) return true;
        return false;
    }

    inline Token peek()
    {
        return tokens.at(current);
    }

    inline Token previous()
    {
        return tokens.at(current -1);
    }


    inline Token consume(TokenType type , std::string_view messg)
    {
        if(check(type)) return advance();

        throw error(peek() , messg);
    }

    inline ParseError error(Token token , std::string_view message)
    {
        Lox::error(token , message);
        return ParseError{""};
    }

    inline void synchronize()
    {
        advance();

        while(!isAtEnd())
        {
            if(previous().type == TSEMICOLON) return;

            switch(peek().type)
            {
                case TCLASS:
                case TFUN:
                case TVAR:
                case TFOR:
                case TIF:
                case TWHILE:
                case TPRINT:
                case TRETURN:
                    return;
            }
            advance();
        }
    }

public:
    Parser(const std::vector<Token>& _tokens)
        : tokens(_tokens) {}
    
    std::vector<std::shared_ptr<Stmt>> parse();
    
};