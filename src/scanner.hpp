#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "error_handler.hpp"

namespace lox
{
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

class Token
{
public:
    TokenType type;
    std::string lexeme;
    int line;

    Token(const TokenType type_, const std::string &lexeme_, const int line_)
        : type(type_), lexeme(lexeme_), line(line_) {}

    virtual std::unique_ptr<Token> clone() const
    {
        return std::unique_ptr<Token>(new Token(type, lexeme, line));
    }

    virtual std::string toString() const
    {
        return lexeme;
    }
};

class StrToken : public Token
{
public:
    std::string literal;

    StrToken(const std::string &lexeme_, const std::string &literal_, const int line_)
        : Token(TokenType::STRING, lexeme_, line_), literal(literal_) {}

    std::unique_ptr<Token> clone() const override
    {
        return std::unique_ptr<StrToken>(new StrToken(lexeme, literal, line));
    }

    std::string toString() const override
    {
        return literal;
    }
};

class NumToken : public Token
{
public:
    double literal;

    NumToken(const std::string &lexeme_, const double literal_, const int line_)
        : Token(TokenType::NUMBER, lexeme_, line_), literal(literal_) {}

    std::unique_ptr<Token> clone() const override
    {
        return std::unique_ptr<NumToken>(new NumToken(lexeme, literal, line));
    }

    std::string toString() const override
    {
        return std::to_string(literal);
    }
};

class Scanner
{
public:
    Scanner(const std::string &source_, ErrorHandler &handler_);
    std::vector<std::unique_ptr<Token>> scanTokens();

private:
    std::unordered_map<std::string, TokenType> keywords;

    std::string source;
    std::vector<std::unique_ptr<Token>> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

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