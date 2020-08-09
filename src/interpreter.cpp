#include <iostream>

#include "interpreter.hpp"

using namespace lox;

void Interpreter::interpret(StmtList &statements)
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
    EnvPtr new_env = std::make_shared<Env>(env);
    executeBlock(stmt->statements, new_env);
}

void Interpreter::executeBlock(StmtList &statements_, EnvPtr env_)
{
    EnvPtr previous = env;

    env = env_;
    for (auto &stmt : statements_)
    {
        execute(stmt.get());
    }
    env = previous;
}

void Interpreter::visit(ClassStmt *stmt) // Testing now
{
    std::unique_ptr<ClassObj> klass = std::unique_ptr<ClassObj>(new ClassObj(stmt, env));
    for (std::shared_ptr<Stmt> method_ : stmt->methods)
    {
        FuncStmt *method = static_cast<FuncStmt *>(method_.get());
        std::unique_ptr<Object> function = std::unique_ptr<FuncObj>(new FuncObj(method, env));
        klass->methods[method->name->lexeme] = std::move(function);
    }

    env->define(stmt->name->lexeme, std::move(klass));
}

void Interpreter::visit(FuncStmt *stmt)
{
    std::unique_ptr<Object> function = std::unique_ptr<FuncObj>(new FuncObj(stmt, env));
    env->define(stmt->name->lexeme, std::move(function));
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

void Interpreter::visit(ReturnStmt *stmt)
{
    if (stmt->value != nullptr)
    {
        value = evaluate(stmt->value.get());
    }
    else
        value = std::unique_ptr<Object>(new NilObj());
    throw BoolObj(true);
}

ObjPtr Interpreter::evaluate(Expr *expr)
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
    ObjPtr left = ObjPtr(evaluate(expr->left.get()));
    ObjPtr right = ObjPtr(evaluate(expr->right.get()));

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

void Interpreter::visit(CallExpr *expr)
{
    ObjPtr callee = evaluate(expr->callee.get());
    ObjList arguments;
    for (auto arg : expr->arguments)
        arguments.push_back(evaluate(arg.get()));

    if (callee->type == ObjectType::FuncType)
    {
        FuncObj *callFunc = static_cast<FuncObj *>(callee.get());
        call(callFunc, std::move(arguments));
    }
    else if (callee->type == ObjectType::ClassType)
    {
        ClassObj *callClass = static_cast<ClassObj *>(callee.get());
        value = instance(callClass, std::move(arguments));
    }
}

void Interpreter::call(FuncObj *callfunc, ObjList &&arguments)
{
    EnvPtr new_env = std::make_shared<Env>(callfunc->closure);
    EnvPtr previous = env;

    for (size_t i = 0; i < callfunc->declaration->params.size(); i++)
    {
        new_env->define(callfunc->declaration->params[i].get()->lexeme, std::move(arguments[i]));
    }

    try
    {
        executeBlock(callfunc->declaration->body, new_env);
    }
    catch (BoolObj &)
    {
        ///////// destructor test /////////
        if (value->type != ObjectType::FuncType)
        {
            for (auto &item : env->values)
            {
                if (item.second->type == ObjectType::ClassType)
                {
                    std::cout << "Destruct class: " + item.first << std::endl;
                    env->values.erase(item.first);
                    break;
                }
            }
        }

        ///////////////////////////////////

        env = previous;
        return;
    }
}

ObjPtr Interpreter::instance(ClassObj *callfunc, ObjList &&arguments)
{
    EnvPtr new_env = std::make_shared<Env>(callfunc->closure);
    EnvPtr previous = env;

    std::cout << "Instance class: " + callfunc->toString() << std::endl;

    return env->get(callfunc->declaration->name->lexeme)->clone();
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
    ObjPtr left = ObjPtr(evaluate(expr->left.get()));

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
    ObjPtr right = ObjPtr(evaluate(expr->right.get()));

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

void Interpreter::visit(GetExpr *expr)
{
    value = evaluate(expr->object.get());
    ClassObj *object = static_cast<ClassObj *>(value.get());
    value = getMember(object, expr->name->lexeme);
}

ObjPtr Interpreter::getMember(ClassObj *object, std::string name)
{
    return object->methods.find(name)->second->clone();
}