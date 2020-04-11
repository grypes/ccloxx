#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "object.hpp"
#include "env.hpp"
#include "parser.hpp"

namespace lox
{

using ObjPtr = std::unique_ptr<Object>;
using ObjList = std::vector<ObjPtr>;
using EnvPtr = std::shared_ptr<Env>;

class Interpreter : public ExprVisitor, StmtVisitor
{
public:
    EnvPtr env;
    ObjPtr value;

    Interpreter() : value(nullptr), env(new Env()) {}

    void interpret(StmtList &statements);
    

private:
    void execute(Stmt *stmt);

    ObjPtr evaluate(Expr *expr);

    void call(FuncObj *callfunc, ObjList &&arguments);

    /// Expressions.
    void visit(AssignExpr *expr) override;
    void visit(BinaryExpr *expr) override;
    void visit(CallExpr *expr) override;
    void visit(GroupingExpr *expr) override;
    void visit(NilLiteralExpr *expr) override;
    void visit(BoolLiteralExpr *expr) override;
    void visit(StrLiteralExpr *expr) override;
    void visit(NumLiteralExpr *expr) override;
    void visit(LogicExpr *expr) override;
    void visit(UnaryExpr *expr) override;
    void visit(VarExpr *expr) override;

    /// Statements.
    void visit(BlockStmt *stmt) override;
    void executeBlock(StmtList &statements_, EnvPtr env_);
    void visit(ExprStmt *stmt) override;
    void visit(FuncStmt *stmt) override;
    void visit(IfStmt *stmt) override;
    void visit(PrintStmt *stmt) override;
    void visit(ReturnStmt *stmt) override;
    void visit(VarStmt *stmt) override;
    void visit(WhileStmt *stmt) override;
};
} // namespace lox

#endif