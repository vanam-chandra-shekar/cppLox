#include "Scanner.hpp"
#include "Lox.hpp"
#include "TokenType.hpp"
#include <unordered_map>

// a Map of all KeyWords 
static const std::unordered_map< std::string, TokenType> keyWords
{
    {"and",    TAND},
    {"class",  TCLASS},
    {"else",   TELSE},
    {"false",  TFALSE},
    {"for",    TFOR},
    {"fun",    TFUN},
    {"if",     TIF},
    {"nil",    TNIL},
    {"or",     TOR},
    {"print",  TPRINT},
    {"return", TRETURN},
    {"super",  TSUPER},
    {"this",   TTHIS},
    {"true",   TTRUE},
    {"var",    TVAR},
    {"while",  TWHILE}
};




std::vector<Token>& Scanner::scanTokens()
{  

    while (!isAtEnd()) {
        //at start of next Lexeme.
        start = current;
        scanToken();
    }

    tokens.emplace_back(TEOF , "" , nullptr , line);
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.size();
}


char Scanner::advance()
{
    return source.at(current++);
}

void Scanner::addToken(TokenType type , Object literal)
{
    //current-start says length of the substring
    std::string sub = source.substr(start , current - start);
    tokens.emplace_back(type , sub , literal , line);
}

void Scanner::addToken(TokenType type)
{
    addToken(type , nullptr);
}

bool Scanner::match(char expected)
{
    if(isAtEnd()) return false;
    if(source.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if(isAtEnd()) return '\0';

    return source.at(current);
}

char Scanner::peekNext()
{
    if(current+1 >= source.size()) return '\0';

    return source.at(current+1);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c<='9';
}

bool Scanner::isAlpha(char c)
{
    return ((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_');
}

bool Scanner::isAlphaNum(char c)
{
    return (isDigit(c) || isAlpha(c));
}

void Scanner::string()
{
    //consuming characters till " apper or till end of source
    while(peek() != '"' && !isAtEnd())
    {
        if(peek() == '\n') line++;
        advance();
    }

    if(isAtEnd())
    {
        Lox::error(line , "Unterminated String.");
        return;
    }

    advance();

    std::string sub = source.substr(start+1 , current-start-2);

    addToken(TSTRING , sub);

}

void Scanner::number()
{
    while(isDigit(peek())) advance();

    if(peek() == '.' && isDigit(peekNext()))
    {
        advance();

        while(isDigit(peek())) advance();
    }

    std::string lexem = source.substr(start ,current - start);
    addToken(TNUMBER , std::stod(lexem));
}

void Scanner::identifier()
{
    while(isAlphaNum(peek())) advance();

    TokenType type = TIDENTIFIER;

    std::string id = source.substr(start , current - start); 
    if(keyWords.find(id) != keyWords.end())
    {
        type = keyWords.at(id);
    }

    addToken(type);
}


void Scanner::scanToken()
{
    char c = advance();

    switch (c) {
        case '(': addToken(TLEFT_PAREN); break;
        case ')': addToken(TRIGHT_PAREN); break;
        case '{': addToken(TLEFT_BRACE); break;
        case '}': addToken(TRIGHT_BRACE); break;
        case ',': addToken(TCOMMA); break;
        case '.': addToken(TDOT); break;
        case '-': addToken(TMINUS); break;
        case '+': addToken(TPLUS); break;
        case ';': addToken(TSEMICOLON); break;
        case '*': addToken(TSTAR); break;

        case '!':
            addToken((match('='))? TBANG_EQUAL : TBANG); 
            break;
        
        case '=':
            addToken((match('='))? TEQUAL_EQUAL : TEQUAL);
            break;
        
        case '<':
            addToken((match('='))? TLESS_EQUAL : TLESS);
            break;
        
        case '>':
            addToken((match('='))? TGREATER_EQUAL : TGREATER);
            break;
        
        case '/':
            if(match('/'))
            {
                while (peek() != '\n' && !isAtEnd()) advance();
            }
            else {
                addToken(TSLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            //skiping whitespaces;
            break;


        case '\n':
            line++;
            break;

        case '"': string(); break;



        default:

            if(isDigit(c))
            {
                number();
            }
            else if(isAlpha(c))
            {
                identifier();
            }
            else{
                Lox::error(line, "Unexpected character .");
            }
    }

}