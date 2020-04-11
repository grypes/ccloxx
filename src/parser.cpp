#include <memory>

#include "parser.hpp"

using namespace lox;

StmtList Parser::parse()
{
    while (!isAtEnd())
    {
        StmtPtr stmt = declaration();
        if (stmt)
            statements.push_back(stmt);
    }

    return statements;
}

StmtPtr Parser::declaration()
{
    if (match(TokenType::FUN))
        return function("function");
    else if (match(TokenType::VAR))
        return varDecl();

    return statement();
}

StmtPtr Parser::function(const std::string &type)
{
    TokenPtr name = consume(TokenType::IDENTIFIER, "Expect " + type + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + type + " name.");
    TokenList parameters;
    if (!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 127)
            {
                errorhandler.add(peek()->line, peek()->lexeme, "Cannot have more than 127 arguments.");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));

        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + type + " body.");
    StmtList body = blocks();
    return std::make_shared<FuncStmt>(name, std::move(parameters), std::move(body));
}

StmtPtr Parser::varDecl()
{
    TokenPtr name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    if (!name)
        return nullptr;

    ExprPtr initializer = nullptr;
    if (match(TokenType::EQUAL))
        initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

StmtPtr Parser::statement()
{
    if (match(TokenType::IF))
        return ifStatement();
    if (match(TokenType::FOR))
        return forStatement();
    if (match(TokenType::WHILE))
        return whileStatement();
    if (match(TokenType::PRINT))
        return printStatement();
    if (match(TokenType::RETURN))
        return returnStatement();
    if (match(TokenType::LEFT_BRACE))
    {
        return std::make_shared<BlockStmt>(std::move(blocks()));
    }

    return expressionStatement();
}

StmtPtr Parser::ifStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'."))
        return nullptr;
    ExprPtr condition = expression();
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition."))
        return nullptr;

    StmtPtr thenBranch = statement();
    StmtPtr elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = statement();
    }

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

StmtPtr Parser::forStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'."))
        return nullptr;

    StmtPtr initializer;
    if (match(TokenType::SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(TokenType::VAR))
    {
        initializer = varDecl();
    }
    else
    {
        initializer = expressionStatement();
    }

    ExprPtr condition = nullptr;
    if (!check(TokenType::SEMICOLON))
    {
        condition = expression();
    }
    if (!consume(TokenType::SEMICOLON, "Expect ';' after loop condition."))
        return nullptr;

    StmtPtr increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN))
    {
        increment = std::make_shared<ExprStmt>(expression());
    }
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses."))
        return nullptr;

    StmtPtr body = statement();

    if (increment != nullptr)
    {
        StmtList statements_;
        statements_.push_back(body);
        statements_.push_back(increment);
        body = std::make_shared<BlockStmt>(std::move(statements_));
    }

    if (condition == nullptr)
        condition = std::make_shared<BoolLiteralExpr>(true);
    body = std::make_shared<WhileStmt>(condition, body);

    if (initializer != nullptr)
    {
        StmtList statements_;
        statements_.push_back(initializer);
        statements_.push_back(body);
        body = std::make_shared<BlockStmt>(std::move(statements_));
    }

    return body;
}

StmtPtr Parser::whileStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'."))
        return nullptr;
    ExprPtr condition = expression();
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition."))
        return nullptr;
    StmtPtr body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

StmtPtr Parser::printStatement()
{
    ExprPtr value = expression();
    if (!consume(TokenType::SEMICOLON, "Expect ';' after value."))
        return nullptr;
    return std::make_shared<PrintStmt>(value);
}

StmtPtr Parser::returnStatement()
{
    TokenPtr keyword = releasePrevious();
    ExprPtr value = nullptr;
    if (!check(TokenType::SEMICOLON))
    {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

StmtPtr Parser::expressionStatement()
{
    ExprPtr expr = expression();
    if (!consume(TokenType::SEMICOLON, "Expect ';' after expression."))
        return nullptr;
    return std::make_shared<ExprStmt>(expr);
}

StmtList Parser::blocks()
{
    StmtList statements_;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
        statements_.push_back(declaration());

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");

    return statements_;
}

ExprPtr Parser::assignment()
{
    ExprPtr expr = logicOr();

    if (match(TokenType::EQUAL))
    {
        TokenPtr equals = releasePrevious();
        ExprPtr value = assignment();
        if (!value)
            return nullptr;

        switch (expr->type)
        {
        case ExprType::VarExprType:
        {
            TokenPtr name = static_cast<VarExpr *>(expr.get())->name;
            expr = std::make_shared<AssignExpr>(name, value);
            break;
        }
        default:
            return nullptr;
            break;
        }
    }

    return expr;
}

ExprPtr Parser::logicOr()
{
    ExprPtr expr = logicAnd();

    while (match(TokenType::OR))
    {
        TokenPtr op = releasePrevious();
        ExprPtr right = logicAnd();
        if (!right)
            return nullptr;
        expr = std::make_shared<LogicExpr>(expr, op, right);
    }
    return expr;
}

ExprPtr Parser::logicAnd()
{
    ExprPtr expr = equality();

    while (match(TokenType::AND))
    {
        TokenPtr op = releasePrevious();
        ExprPtr right = equality();
        if (!right)
            return nullptr;
        expr = std::make_shared<LogicExpr>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::equality()
{
    ExprPtr expr = comparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
    {
        TokenPtr opr = releasePrevious();
        ExprPtr right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, opr, right);
    }

    return expr;
}

ExprPtr Parser::comparison()
{
    ExprPtr expr = addition();

    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        TokenPtr opr = releasePrevious();
        ExprPtr right = addition();
        expr = std::make_shared<BinaryExpr>(expr, opr, right);
    }

    return expr;
}

ExprPtr Parser::addition()
{
    ExprPtr expr = multiplication();

    while (match(TokenType::MINUS, TokenType::PLUS))
    {
        TokenPtr opr = releasePrevious();
        ExprPtr right = multiplication();
        expr = std::make_shared<BinaryExpr>(expr, opr, right);
    }

    return expr;
}

ExprPtr Parser::multiplication()
{
    ExprPtr expr = unary();

    while (match(TokenType::SLASH, TokenType::STAR))
    {
        TokenPtr opr = releasePrevious();
        ExprPtr right = unary();
        expr = std::make_shared<BinaryExpr>(expr, opr, right);
    }

    return expr;
}

ExprPtr Parser::unary()
{
    if (match(TokenType::BANG, TokenType::MINUS))
    {
        TokenPtr opr = releasePrevious();
        ExprPtr right = unary();
        return std::make_shared<UnaryExpr>(opr, right);
    }

    return call();
}

ExprPtr Parser::call()
{
    ExprPtr expr = primary();

    while (true)
    {
        if (match(TokenType::LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else
        {
            break;
        }
    }

    return expr;
}

ExprPtr Parser::finishCall(ExprPtr callee)
{
    ExprList arguments;

    if (!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 127)
                errorhandler.add(peek()->line, peek()->lexeme, "Cannot have more than 127 arguments.");

            ExprPtr expr = expression();
            if (expr)
                arguments.push_back(expr);
            else
                return nullptr;
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_shared<CallExpr>(callee, std::move(arguments));
}

ExprPtr Parser::primary()
{
    if (match(TokenType::FALSE))
        return std::make_shared<BoolLiteralExpr>(false);
    if (match(TokenType::TRUE))
        return std::make_shared<BoolLiteralExpr>(true);
    if (match(TokenType::NIL))
        return std::make_shared<NilLiteralExpr>();

    if (match(TokenType::NUMBER))
    {
        double literal = static_cast<NumToken *>(previous())->literal;
        return std::make_shared<NumLiteralExpr>(literal);
    }
    if (match(TokenType::STRING))
    {
        std::string literal = static_cast<StrToken *>(previous())->literal;
        return std::make_shared<StrLiteralExpr>(literal);
    }

    if (match(TokenType::IDENTIFIER))
    {
        return std::make_shared<VarExpr>(releasePrevious());
    }

    if (match(TokenType::LEFT_PAREN))
    {
        ExprPtr expr = expression();
        if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after expression."))
            return nullptr;
        return std::make_shared<GroupingExpr>(expr);
    }

    return nullptr;
}

TokenPtr Parser::consume(TokenType type_, const std::string &error_message)
{
    if (check(type_))
    {
        advance();
        return releasePrevious();
    }

    errorhandler.add(peek()->line, peek()->lexeme, error_message);
    return nullptr;
}

template <typename... TokenT>
bool Parser::match(TokenT... types)
{
    std::vector<TokenType> typevec = {types...};

    for (TokenType type : typevec)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek()->type == type;
}

Token *Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek()->type == TokenType::END_OF_FILE;
}

Token *Parser::peek()
{
    return tokens[current].get();
}

Token *Parser::previous()
{
    return tokens[current - 1].get();
}

TokenPtr Parser::releasePrevious()
{
    return tokens[current - 1];
}