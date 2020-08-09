#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "scanner.hpp"
#include "ast.hpp"

namespace lox
{
    using ExprPtr = std::shared_ptr<Expr>;
    using StmtPtr = std::shared_ptr<Stmt>;

    using ExprList = std::vector<ExprPtr>;
    using StmtList = std::vector<StmtPtr>;

    class Parser
    {
    private:
        TokenList tokens;
        StmtList statements;
        size_t current = 0;

    public:
        Parser(TokenList &&tokens_, ErrorHandler &error_) : tokens(tokens_), errorhandler(error_) {}

        StmtList parse();

    private:
        template <typename... TokenT>
        bool match(TokenT... types);

        ErrorHandler &errorhandler;

        StmtPtr declaration();

        StmtPtr classDecl(const std::string &type);

        StmtPtr function(const std::string &type);

        StmtPtr varDecl();

        StmtPtr statement();

        StmtPtr ifStatement();

        StmtPtr forStatement();

        StmtPtr whileStatement();

        StmtPtr printStatement();

        StmtPtr returnStatement();

        StmtList blocks();

        StmtPtr expressionStatement();

        ExprPtr assignment();

        ExprPtr logicOr();

        ExprPtr logicAnd();

        ExprPtr equality();

        ExprPtr comparison();

        ExprPtr addition();

        ExprPtr multiplication();

        ExprPtr unary();

        ExprPtr call();

        ExprPtr finishCall(ExprPtr callee);

        ExprPtr primary();

        bool check(TokenType type);

        Token *advance();

        bool isAtEnd();

        Token *peek();

        Token *previous();

        TokenPtr releasePrevious();

        ExprPtr expression()
        {
            return assignment();
        }

        TokenPtr consume(TokenType type_, const std::string &error_message);
    };
} // namespace lox

#endif