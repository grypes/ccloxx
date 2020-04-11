#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "error_handler.hpp"

namespace lox
{

/***************************************************/
// Token

enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    END_OF_FILE
};

inline const std::string tokentype_to_string(const TokenType &type)
{
    switch (type)
    {
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::DOT:
        return "DOT";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::STAR:
        return "STAR";
    case TokenType::BANG:
        return "BANG";
    case TokenType::BANG_EQUAL:
        return "BANG_EQUAL";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case TokenType::GREATER:
        return "GREATER";
    case TokenType::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TokenType::LESS:
        return "LESS";
    case TokenType::LESS_EQUAL:
        return "LESS_EQUAL";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::AND:
        return "AND";
    case TokenType::CLASS:
        return "CLASS";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::FALSE:
        return "FALSE";
    case TokenType::FUN:
        return "FUN";
    case TokenType::FOR:
        return "FOR";
    case TokenType::IF:
        return "IF";
    case TokenType::NIL:
        return "NIL";
    case TokenType::OR:
        return "OR";
    case TokenType::PRINT:
        return "PRINT";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::SUPER:
        return "SUPER";
    case TokenType::THIS:
        return "THIS";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::VAR:
        return "VAR";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::END_OF_FILE:
        return "END";
    default:
        return "???";
    }
}

class Token
{
    friend const std::string tokentype_to_string(const TokenType &type);

public:
    TokenType type;
    std::string lexeme;
    size_t line;

    Token(const TokenType type_, const std::string &lexeme_, const size_t line_)
        : type(type_), lexeme(lexeme_), line(line_) {}

    virtual std::string toString() const
    {
        return "Type: " + tokentype_to_string(type) + ", lexeme: " + lexeme + ";";
    }
};

class StrToken : public Token
{
public:
    std::string literal;

    StrToken(const std::string &lexeme_, const std::string &literal_, const size_t line_)
        : Token(TokenType::STRING, lexeme_, line_), literal(literal_) {}

    std::string toString() const override
    {
        return "Type: " + tokentype_to_string(TokenType::STRING) + ", literal: " + literal + ";";
    }
};

class NumToken : public Token
{
public:
    double literal;

    NumToken(const std::string &lexeme_, const double literal_, const size_t line_)
        : Token(TokenType::NUMBER, lexeme_, line_), literal(literal_) {}

    std::string toString() const override
    {
        return "Type: " + tokentype_to_string(TokenType::NUMBER) + ", literal: " + std::to_string(literal) + ";";
    }
};

/***************************************************/
// Scanner

using TokenPtr = std::shared_ptr<Token>;
using TokenList = std::vector<TokenPtr>;

class Scanner
{
public:
    Scanner(const std::string &source_, ErrorHandler &handler_);
    TokenList scanTokens();

private:
    std::unordered_map<std::string, TokenType> keywords;

    std::string source;
    TokenList tokens;
    size_t start = 0;
    size_t current = 0;
    size_t line = 1;

    ErrorHandler &errorhandler;

    bool isAtEnd();
    void scanToken();
    void getString();
    void getNum();

    char advance();
    char peek();
    char peekNext();
    bool match(const char &c);
    bool isDigit(const char &c);
    bool isAlpha(const char &c);
    bool isAlphaNumeric(const char &c);
    void identifer();

    void addToken(TokenType type);
    void addNumToken(const std::string &literal);
    void addStrToken(const std::string &literal);
    void addToken(TokenType type, const std::string &literal);
};
} // namespace lox
#endif