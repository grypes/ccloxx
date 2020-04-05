#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "scanner.hpp"
#include "ast.hpp"

namespace lox
{
class Parser
{
private:
    std::vector<std::unique_ptr<Token>> tokens;
    std::vector<std::shared_ptr<Stmt>> statements;
    size_t current = 0;

public:
    Parser(std::vector<std::unique_ptr<Token>> tokens_, ErrorHandler &error_) : tokens(std::move(tokens_)), errorhandler(error_) {}

    std::vector<std::shared_ptr<Stmt>> parse();

private:
    template <typename... TokenT>
    bool match(TokenT... types);

    ErrorHandler &errorhandler;

    std::unique_ptr<Stmt> declaration();

    std::unique_ptr<Stmt> classDecl();

    std::unique_ptr<Stmt> function();

    std::unique_ptr<Stmt> varDecl();

    std::unique_ptr<Stmt> statement();

    std::unique_ptr<Stmt> ifStatement();

    std::unique_ptr<Stmt> forStatement();

    std::unique_ptr<Stmt> whileStatement();

    std::unique_ptr<Stmt> printStatement();

    std::vector<std::unique_ptr<Stmt>> blocks();

    std::unique_ptr<Stmt> expressionStatement();

    std::unique_ptr<Expr> assignment();

    std::unique_ptr<Expr> logicOr();

    std::unique_ptr<Expr> logicAnd();

    std::unique_ptr<Expr> equality();

    std::unique_ptr<Expr> comparison();

    std::unique_ptr<Expr> addition();

    std::unique_ptr<Expr> multiplication();

    std::unique_ptr<Expr> unary();

    std::unique_ptr<Expr> call();

    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);

    std::unique_ptr<Expr> primary();

    bool check(TokenType type);

    Token *advance();

    bool isAtEnd();

    Token *peek();

    Token *previous();

    std::unique_ptr<Token> releasePrevious();

    std::unique_ptr<Expr> expression()
    {
        return assignment();
    }

    std::unique_ptr<Token> consume(TokenType type_, const std::string &error_message);
};
} // namespace lox

#endif