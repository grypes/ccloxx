#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "object.hpp"
#include "env.hpp"

namespace lox
{
class Interpreter : public ExprVisitor, StmtVisitor
{
    std::unique_ptr<Object> value;
    std::shared_ptr<Env> env;

public:
    Interpreter() : value(nullptr), env(new Env()) {}

    void interpret(std::vector<std::shared_ptr<Stmt>> statements);

private:
    void execute(Stmt *stmt);

    std::unique_ptr<Object> evaluate(Expr *expr);

    /// Expressions.
    void visit(AssignExpr *expr) override;
    void visit(BinaryExpr *expr) override;
    // void visit(CallExpr *expr) override;
    // void visit(GetExpr *expr) override;
    void visit(GroupingExpr *expr) override;
    void visit(NilLiteralExpr *expr) override;
    void visit(BoolLiteralExpr *expr) override;
    void visit(StrLiteralExpr *expr) override;
    void visit(NumLiteralExpr *expr) override;
    void visit(LogicExpr *expr) override;
    // void visit(SetExpr *expr) override;
    // void visit(SuperExpr *expr) override;
    // void visit(ThisExpr *expr) override;
    void visit(UnaryExpr *expr) override;
    void visit(VarExpr *expr) override;

    /// Statements.
    void visit(BlockStmt *stmt) override;
    void executeBlock(std::vector<std::shared_ptr<Stmt>> statements_, std::shared_ptr<Env> env_);

    // void visit(ClassStmt *stmt) override;
    void visit(ExprStmt *stmt) override;
    // void visit(FuncStmt *stmt) override;
    void visit(IfStmt *stmt) override;
    void visit(PrintStmt *stmt) override;
    // void visit(ReturnStmt *stmt) override;
    void visit(VarStmt *stmt) override;
    void visit(WhileStmt *stmt) override;
};
} // namespace lox

#endif