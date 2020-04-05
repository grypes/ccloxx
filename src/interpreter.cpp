#include <iostream>

#include "interpreter.hpp"

using namespace lox;

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
    for (auto &stmt : statements)
        execute(stmt.get());
}

void Interpreter::execute(Stmt *stmt)
{
    stmt->accept(*this);
}

void Interpreter::visit(BlockStmt *stmt)
{
    std::shared_ptr<Env> new_env = std::make_shared<Env>(env);
    executeBlock(stmt->statements, new_env);
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements_, std::shared_ptr<Env> env_)
{
    std::shared_ptr<Env> previous = env;

    env = env_;
    for (auto &stmt : statements_)
    {
        execute(stmt.get());
    }
    env = previous;
}

void Interpreter::visit(ExprStmt *stmt)
{
    value = evaluate(stmt->expression.get());
}

void Interpreter::visit(IfStmt *stmt)
{
    value = evaluate(stmt->condition.get());
    if (value->isTrue())
        execute(stmt->thenBranch.get());
    else if (stmt->elseBranch)
        execute(stmt->elseBranch.get());
    value = nullptr;
}

void Interpreter::visit(PrintStmt *stmt)
{
    value = evaluate(stmt->expression.get());
    if (value)
    {
        std::cout << value->toString() << std::endl;
    }
    value = nullptr;
}

void Interpreter::visit(VarStmt *stmt)
{
    if (stmt->initializer)
        value = evaluate(stmt->initializer.get());
    env->define(stmt->name->lexeme, value->clone());
    value = nullptr;
}

void Interpreter::visit(WhileStmt *stmt)
{
    value = evaluate(stmt->condition.get());
    while (value->isTrue())
    {
        execute(stmt->body.get());
        value = evaluate(stmt->condition.get());
    }
    value = nullptr;
}

std::unique_ptr<Object> Interpreter::evaluate(Expr *expr)
{
    expr->accept(*this);
    return std::move(value);
}

void Interpreter::visit(AssignExpr *expr)
{
    value = evaluate(expr->value.get());
    env->assign(expr->name->lexeme, value->clone());
}

void Interpreter::visit(BinaryExpr *expr)
{
    std::unique_ptr<Object> left = std::unique_ptr<Object>(evaluate(expr->left.get()));
    std::unique_ptr<Object> right = std::unique_ptr<Object>(evaluate(expr->right.get()));

    switch (expr->op->type)
    {
    case TokenType::GREATER:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new BoolObj(leftValue > rightValue));
        break;
    }
    case TokenType::GREATER_EQUAL:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new BoolObj(leftValue >= rightValue));
        break;
    }
    case TokenType::LESS:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new BoolObj(leftValue < rightValue));
        break;
    }
    case TokenType::LESS_EQUAL:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new BoolObj(leftValue <= rightValue));
        break;
    }
    case TokenType::BANG_EQUAL:
    {
        value.reset(new BoolObj(!left->equals(right.get())));
        break;
    }
    case TokenType::EQUAL_EQUAL:
    {
        value.reset(new BoolObj(left->equals(right.get())));
        break;
    }
    case TokenType::MINUS:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new NumObj(leftValue - rightValue));
        break;
    }
    case TokenType::PLUS:
    {
        if (left->type == ObjectType::NumType && right->type == ObjectType::NumType)
        {
            double leftValue = static_cast<NumObj *>(left.get())->value;
            double rightValue = static_cast<NumObj *>(right.get())->value;
            value.reset(new NumObj(leftValue + rightValue));
        }

        if (left->type == ObjectType::StrType && right->type == ObjectType::StrType)
        {
            const std::string &leftValue = static_cast<StrObj *>(left.get())->value;
            const std::string &rightValue =
                static_cast<StrObj *>(right.get())->value;
            value.reset(new StrObj(leftValue + rightValue));
        }

        break;
    }
    case TokenType::SLASH:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new NumObj(leftValue / rightValue));
        break;
    }
    case TokenType::STAR:
    {
        double leftValue = static_cast<NumObj *>(left.get())->value;
        double rightValue = static_cast<NumObj *>(right.get())->value;
        value.reset(new NumObj(leftValue * rightValue));
        break;
    }
    default:
        break;
    }
}

void Interpreter::visit(GroupingExpr *expr)
{
    value = evaluate(expr->expression.get());
}

void Interpreter::visit(BoolLiteralExpr *expr)
{
    value.reset(new BoolObj(expr->literal));
}

void Interpreter::visit(NilLiteralExpr *expr)
{
    value.reset(new NilObj());
}

void Interpreter::visit(NumLiteralExpr *expr)
{
    value.reset(new NumObj(expr->literal));
}

void Interpreter::visit(StrLiteralExpr *expr)
{
    value.reset(new StrObj(expr->literal));
}

void Interpreter::visit(LogicExpr *expr)
{
    std::unique_ptr<Object> left = std::unique_ptr<Object>(evaluate(expr->left.get()));

    value.reset(new BoolObj(left->isTrue()));

    if (expr->opr->type == TokenType::OR && !left->isTrue())
    {
        value = evaluate(expr->right.get());
    }

    if (expr->opr->type == TokenType::AND && left->isTrue())
    {
        value = evaluate(expr->right.get());
    }
}

void Interpreter::visit(UnaryExpr *expr)
{
    std::unique_ptr<Object> right = std::unique_ptr<Object>(evaluate(expr->right.get()));

    switch (expr->op->type)
    {
    case TokenType::BANG:
    {
        BoolObj *result = static_cast<BoolObj *>(right.get());
        value.reset(new BoolObj(!result->isTrue()));
        break;
    }
    case TokenType::MINUS:
    {
        NumObj *result = static_cast<NumObj *>(right.get());
        value.reset(new NumObj(-result->value));
        break;
    }
    default:
        break;
    }
}

void Interpreter::visit(VarExpr *expr)
{
    value = env->get(expr->name->lexeme)->clone();
}
