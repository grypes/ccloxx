#include <memory>

#include "parser.hpp"

using namespace lox;

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
    while (!isAtEnd())
    {
        std::unique_ptr<Stmt> stmt = declaration();
        if (stmt)
            statements.push_back(std::move(stmt));
    }

    return statements;
}

std::unique_ptr<Stmt> Parser::declaration()
{
    if (match(TokenType::VAR))
        return varDecl();

    return statement();
}

std::unique_ptr<Stmt> Parser::varDecl()
{
    std::unique_ptr<Token> name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    if (!name)
        return nullptr;

    std::unique_ptr<Expr> initializer = nullptr;
    if (match(TokenType::EQUAL))
        initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::unique_ptr<VarStmt>(new VarStmt(std::move(name), std::move(initializer)));
}

std::unique_ptr<Stmt> Parser::statement()
{
    if (match(TokenType::IF))
        return ifStatement();
    if (match(TokenType::FOR))
        return forStatement();
    if (match(TokenType::WHILE))
        return whileStatement();
    if (match(TokenType::PRINT))
        return printStatement();
    if (match(TokenType::LEFT_BRACE))
    {
        std::vector<std::shared_ptr<Stmt>> statements_;
        for (auto &s : blocks())
            statements_.push_back(std::move(s));
        return std::unique_ptr<BlockStmt>(new BlockStmt(statements_));
    }

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::ifStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'."))
        return nullptr;
    std::unique_ptr<Expr> condition = expression();
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition."))
        return nullptr;

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = statement();
    }

    return std::unique_ptr<IfStmt>(new IfStmt(std::move(condition), std::move(thenBranch), std::move(elseBranch)));
}

std::unique_ptr<Stmt> Parser::forStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'."))
        return nullptr;

    std::unique_ptr<Stmt> initializer;
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

    std::unique_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON))
    {
        condition = expression();
    }
    if (!consume(TokenType::SEMICOLON, "Expect ';' after loop condition."))
        return nullptr;

    std::unique_ptr<Stmt> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN))
    {
        increment = std::unique_ptr<ExprStmt>(new ExprStmt(std::move(expression())));
    }
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses."))
        return nullptr;

    std::unique_ptr<Stmt> body = statement();

    if (increment != nullptr)
    {
        std::vector<std::shared_ptr<Stmt>> statements_;
        statements_.push_back(std::move(body));
        statements_.push_back(std::move(increment));
        body = std::unique_ptr<BlockStmt>(new BlockStmt(statements_));
    }

    if (condition == nullptr)
        condition = std::unique_ptr<BoolLiteralExpr>(new BoolLiteralExpr(true));
    body = std::unique_ptr<WhileStmt>(new WhileStmt(std::move(condition), std::move(body)));

    if (initializer != nullptr)
    {
        std::vector<std::shared_ptr<Stmt>> statements_;
        statements_.push_back(std::move(initializer));
        statements_.push_back(std::move(body));
        body = std::unique_ptr<BlockStmt>(new BlockStmt(statements_));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::whileStatement()
{
    if (!consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'."))
        return nullptr;
    std::unique_ptr<Expr> condition = expression();
    if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition."))
        return nullptr;
    std::unique_ptr<Stmt> body = statement();

    return std::unique_ptr<WhileStmt>(new WhileStmt(std::move(condition), std::move(body)));
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    std::unique_ptr<Expr> value = expression();
    if (!consume(TokenType::SEMICOLON, "Expect ';' after value."))
        return nullptr;
    return std::unique_ptr<PrintStmt>(new PrintStmt(std::move(value)));
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    std::unique_ptr<Expr> expr = expression();
    if (!consume(TokenType::SEMICOLON, "Expect ';' after expression."))
        return nullptr;
    return std::unique_ptr<ExprStmt>(new ExprStmt(std::move(expr)));
}

std::vector<std::unique_ptr<Stmt>> Parser::blocks()
{
    std::vector<std::unique_ptr<Stmt>> statements_;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
        statements_.push_back(declaration());

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");

    return statements_;
}

// std::unique_ptr<Stmt> Parser::declaration() {
//     if(match(TokenType::CLASS))
//         return classDecl();
//     else if(match(TokenType::FUN))
//         return function();
//     else if(match(TokenType::VAR))
//         return varDecl();
//     else
//         return statement();

//     return statement();
// }

// std::unique_ptr<Stmt> Parser::classDecl() {
//     std::unique_ptr<Token> name = consume(TokenType::IDENTIFIER, "Expected class name");
//     if(!name)
//         return nullptr;

//     std::unique_ptr<Expr> superclass = nullptr;
//     if(match(TokenType::LESS)) {
//         std::unique_ptr<Token> name = consume(TokenType::IDENTIFIER, "Expect superclass name.");
//         superclass = std::unique_ptr<VarExpr>(new VarExpr(std::move(name)));
//     }

//     if(!consume(TokenType::LEFT_BRACE, "Expected '{' before class body"))
//         return nullptr;
//     std::vector<std::unique_ptr<FuncStmt>> methods;
//     while(match(TokenType::RIGHT_BRACE) && !isAtEnd())
//         methods.push_back(function());

//     consume(TokenType::RIGHT_BRACE, "Expected '}' before class body");
//     return std::unique_ptr<ClassStmt>(new ClassStmt(std::move(name), std::move(superclass), std::move(methods)));
// }

// std::unique_ptr<Stmt> function() {

// };

std::unique_ptr<Expr> Parser::assignment()
{
    std::unique_ptr<Expr> expr = logicOr();

    if (match(TokenType::EQUAL))
    {
        std::unique_ptr<Token> equals = releasePrevious();
        std::unique_ptr<Expr> value = assignment();
        if (!value)
            return nullptr;

        switch (expr->type)
        {
        case ExprType::VarExprType:
        {
            std::unique_ptr<Token> name =
                static_cast<VarExpr *>(expr.get())->name->clone();
            expr = std::unique_ptr<AssignExpr>(new AssignExpr(std::move(name), std::move(value)));
            break;
        }
        // case ExprType::GetExprType:
        // {
        //     GetExpr *variable = static_cast<GetExpr *>(expr.get());
        //     std::unique_ptr<Token> name = variable->name->clone();
        //     std::unique_ptr<Expr> object = variable->obj->clone();
        //     expr = std::unique_ptr<SetExpr>(new SetExpr(std::move(object), std::move(name), std::move(value)));
        //     break;
        // }
        default:
            return nullptr;
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicOr()
{
    std::unique_ptr<Expr> expr = logicAnd();

    while (match(TokenType::OR))
    {
        std::unique_ptr<Token> op = releasePrevious();
        std::unique_ptr<Expr> right = logicAnd();
        if (!right)
            return nullptr;
        expr = std::unique_ptr<LogicExpr>(new LogicExpr(std::move(expr), std::move(op), std::move(right)));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logicAnd()
{
    std::unique_ptr<Expr> expr = equality();

    while (match(TokenType::AND))
    {
        std::unique_ptr<Token> op = releasePrevious();
        std::unique_ptr<Expr> right = equality();
        if (!right)
            return nullptr;
        expr = std::unique_ptr<LogicExpr>(new LogicExpr(std::move(expr), std::move(op), std::move(right)));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> expr = comparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
    {
        std::unique_ptr<Token> opr = releasePrevious();
        std::unique_ptr<Expr> right = comparison();
        expr = std::unique_ptr<Expr>(new BinaryExpr(std::move(expr), std::move(opr), std::move(right)));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> expr = addition();

    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        std::unique_ptr<Token> opr = releasePrevious();
        std::unique_ptr<Expr> right = addition();
        expr = std::unique_ptr<Expr>(new BinaryExpr(std::move(expr), std::move(opr), std::move(right)));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::addition()
{
    std::unique_ptr<Expr> expr = multiplication();

    while (match(TokenType::MINUS, TokenType::PLUS))
    {
        std::unique_ptr<Token> opr = releasePrevious();
        std::unique_ptr<Expr> right = multiplication();
        expr = std::unique_ptr<Expr>(new BinaryExpr(std::move(expr), std::move(opr), std::move(right)));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::multiplication()
{
    std::unique_ptr<Expr> expr = unary();

    while (match(TokenType::SLASH, TokenType::STAR))
    {
        std::unique_ptr<Token> opr = releasePrevious();
        std::unique_ptr<Expr> right = unary();
        expr = std::unique_ptr<Expr>(new BinaryExpr(std::move(expr), std::move(opr), std::move(right)));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match(TokenType::BANG, TokenType::MINUS))
    {
        std::unique_ptr<Token> opr = releasePrevious();
        std::unique_ptr<Expr> right = unary();
        return std::unique_ptr<Expr>(new UnaryExpr(std::move(opr), std::move(right)));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::call()
{
    std::unique_ptr<Expr> expr = primary();

    // while (true)
    // {
    //     // if (match(TokenType::LEFT_PAREN))
    //     // {
    //     //     expr = finishCall(std::move(expr));
    //     // }
    //     // else if (match(TokenType::DOT))
    //     // {
    //     //     if (consume(TokenType::IDENTIFIER, "Expect property name after '.'."))
    //     //         expr = std::unique_ptr<GetExpr>(new GetExpr(std::move(expr), releasePrevious()));
    //     // }
    //     // else
    //     // {
    //     //     break;
    //     // }
    // }

    return expr;
}

// std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee)
// {
//     std::vector<std::unique_ptr<Expr>> arguments;

//     if (!check(TokenType::RIGHT_PAREN))
//     {
//         do
//         {
//             std::unique_ptr<Expr> expr = expression();
//             if (expr)
//                 arguments.push_back(std::move(expr));
//             else
//                 return nullptr;
//         } while (match(TokenType::COMMA));
//     }

//     std::unique_ptr<Token> name = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
//     if (!name)
//         return nullptr;

//     return std::unique_ptr<CallExpr>(new CallExpr(
//         std::move(callee), std::move(name), std::move(arguments)));
// }

std::unique_ptr<Expr> Parser::primary()
{
    if (match(TokenType::FALSE))
        return std::unique_ptr<Expr>(new BoolLiteralExpr(false));
    if (match(TokenType::TRUE))
        return std::unique_ptr<Expr>(new BoolLiteralExpr(true));
    if (match(TokenType::NIL))
        return std::unique_ptr<Expr>(new NilLiteralExpr());

    if (match(TokenType::NUMBER))
    {
        double literal = static_cast<NumToken *>(previous())->literal;
        return std::unique_ptr<NumLiteralExpr>(new NumLiteralExpr(literal));
    }
    if (match(TokenType::STRING))
    {
        std::string literal = static_cast<StrToken *>(previous())->literal;
        return std::unique_ptr<StrLiteralExpr>(new StrLiteralExpr(literal));
    }

    if (match(TokenType::IDENTIFIER))
    {
        return std::unique_ptr<VarExpr>(new VarExpr(releasePrevious()));
    }

    if (match(TokenType::LEFT_PAREN))
    {
        std::unique_ptr<Expr> expr = expression();
        if (!consume(TokenType::RIGHT_PAREN, "Expect ')' after expression."))
            return nullptr;
        return std::unique_ptr<GroupingExpr>(new GroupingExpr(std::move(expr)));
    }

    return nullptr;
}

std::unique_ptr<Token> Parser::consume(TokenType type_, const std::string &error_message)
{
    if (check(type_))
    {
        advance();
        return releasePrevious();
    }

    errorhandler.add(peek()->line, "", error_message);
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

std::unique_ptr<Token> Parser::releasePrevious()
{
    return std::move(tokens[current - 1]);
}