#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>

#include "scanner.hpp"

namespace lox
{

    using TokenPtr = std::shared_ptr<Token>;

    class AssignExpr;
    class BinaryExpr;
    class CallExpr;
    class GroupingExpr;
    class BoolLiteralExpr;
    class NilLiteralExpr;
    class NumLiteralExpr;
    class StrLiteralExpr;
    class LogicExpr;
    class UnaryExpr;
    class VarExpr;
    class GetExpr;

    class ExprVisitor
    {
    public:
        virtual void visit(AssignExpr *expr) = 0;
        virtual void visit(BinaryExpr *expr) = 0;
        virtual void visit(CallExpr *expr) = 0;
        virtual void visit(GroupingExpr *expr) = 0;
        virtual void visit(NilLiteralExpr *expr) = 0;
        virtual void visit(BoolLiteralExpr *expr) = 0;
        virtual void visit(NumLiteralExpr *expr) = 0;
        virtual void visit(StrLiteralExpr *expr) = 0;
        virtual void visit(LogicExpr *expr) = 0;
        virtual void visit(VarExpr *expr) = 0;
        virtual void visit(UnaryExpr *expr) = 0;
        virtual void visit(GetExpr *expr) = 0;
    };

    enum class ExprType
    {
        AssignExprType,
        BinaryExprType,
        CallExprType,
        GroupExprType,
        NilLiteralExprType,
        BoolLiteralExprType,
        NumLiteralExprType,
        StrLiteralExprType,
        LogicalExprType,
        UnaryExprType,
        VarExprType,
        GetExprType
    };

    class Expr
    {
    public:
        ExprType type;

        Expr(ExprType type_) : type(type_) {}

        virtual ~Expr() {}

        virtual void accept(ExprVisitor &visitor) = 0;
    };

    class AssignExpr : public Expr
    {
    public:
        TokenPtr name;
        std::shared_ptr<Expr> value;

        AssignExpr(TokenPtr name_, std::shared_ptr<Expr> value_) : Expr(ExprType::AssignExprType),
                                                                   name(name_),
                                                                   value(value_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class BinaryExpr : public Expr
    {
    public:
        std::shared_ptr<Expr> left;
        TokenPtr op;
        std::shared_ptr<Expr> right;

        BinaryExpr(std::shared_ptr<Expr> left_, TokenPtr op_,
                   std::shared_ptr<Expr> right_) : Expr(ExprType::BinaryExprType),
                                                   left(left_),
                                                   op(op_),
                                                   right(right_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class CallExpr : public Expr
    {
    public:
        std::shared_ptr<Expr> callee;
        std::vector<std::shared_ptr<Expr>> arguments;

        CallExpr(std::shared_ptr<Expr> callee_,
                 std::vector<std::shared_ptr<Expr>> &&arguments_) : Expr(ExprType::CallExprType),
                                                                    callee(callee_),
                                                                    arguments(arguments_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class GroupingExpr : public Expr
    {
    public:
        std::shared_ptr<Expr> expression;

        GroupingExpr(std::shared_ptr<Expr> expression_) : Expr(ExprType::GroupExprType),
                                                          expression(expression_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class NilLiteralExpr : public Expr
    {
    public:
        NilLiteralExpr() : Expr(ExprType::NilLiteralExprType) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class BoolLiteralExpr : public Expr
    {
    public:
        bool literal;

        BoolLiteralExpr(const bool literal_) : Expr(ExprType::BoolLiteralExprType), literal(literal_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class NumLiteralExpr : public Expr
    {
    public:
        double literal;

        NumLiteralExpr(const double literal_) : Expr(ExprType::NumLiteralExprType), literal(literal_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class StrLiteralExpr : public Expr
    {
    public:
        std::string literal;

        StrLiteralExpr(const std::string literal_) : Expr(ExprType::StrLiteralExprType), literal(literal_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class LogicExpr : public Expr
    {
    public:
        std::shared_ptr<Expr> left;
        TokenPtr opr;
        std::shared_ptr<Expr> right;

        LogicExpr(std::shared_ptr<Expr> left_, TokenPtr opr_,
                  std::shared_ptr<Expr> right_) : Expr(ExprType::LogicalExprType),
                                                  left(left_),
                                                  opr(opr_),
                                                  right(right_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class VarExpr : public Expr
    {
    public:
        TokenPtr name;

        VarExpr(TokenPtr name_) : Expr(ExprType::VarExprType),
                                  name(name_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class UnaryExpr : public Expr
    {
    public:
        TokenPtr op;
        std::shared_ptr<Expr> right;

        UnaryExpr(TokenPtr op_, std::shared_ptr<Expr> right_) : Expr(ExprType::UnaryExprType),
                                                                op(op_),
                                                                right(right_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    class GetExpr : public Expr
    {
    public:
        std::shared_ptr<Expr> object;
        TokenPtr name;

        GetExpr(std::shared_ptr<Expr> object_, TokenPtr name_) : Expr(ExprType::GetExprType),
                                                                 object(object_),
                                                                 name(name_) {}

        void accept(ExprVisitor &visitor) override { visitor.visit(this); }
    };

    /*****************************************/

    class BlockStmt;
    class ExprStmt;
    class FuncStmt;
    class ClassStmt;
    class IfStmt;
    class PrintStmt;
    class ReturnStmt;
    class VarStmt;
    class WhileStmt;

    class StmtVisitor
    {
    public:
        virtual void visit(BlockStmt *stmt) = 0;
        virtual void visit(ExprStmt *stmt) = 0;
        virtual void visit(FuncStmt *stmt) = 0;
        virtual void visit(ClassStmt *stmt) = 0;
        virtual void visit(IfStmt *stmt) = 0;
        virtual void visit(PrintStmt *stmt) = 0;
        virtual void visit(ReturnStmt *stmt) = 0;
        virtual void visit(VarStmt *stmt) = 0;
        virtual void visit(WhileStmt *stmt) = 0;
    };

    enum class StmtType
    {
        BlockStmtType,
        ClassStmtType,
        ExprStmtType,
        FuncStmtType,
        IfStmtType,
        PrintStmtType,
        ReturnStmtType,
        VarStmtType,
        WhileStmtType
    };

    class Stmt
    {
    public:
        StmtType type;

        Stmt(StmtType type_) : type(type_) {}

        virtual ~Stmt() {}

        virtual void accept(StmtVisitor &visitor) = 0;
    };

    class ExprStmt : public Stmt
    {
    public:
        std::shared_ptr<Expr> expression;

        ExprStmt(std::shared_ptr<Expr> expression_) : Stmt(StmtType::ExprStmtType),
                                                      expression(expression_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class FuncStmt : public Stmt
    {
    public:
        TokenPtr name;
        TokenList params;
        std::vector<std::shared_ptr<Stmt>> body;

        FuncStmt(TokenPtr name_,
                 TokenList &&params_,
                 std::vector<std::shared_ptr<Stmt>> &&body_) : Stmt(StmtType::FuncStmtType),
                                                               name(name_),
                                                               params(params_),
                                                               body(body_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class ClassStmt : public Stmt
    {
    public:
        TokenPtr name;
        std::vector<std::shared_ptr<Stmt>> methods;

        ClassStmt(TokenPtr name_,
                  std::vector<std::shared_ptr<Stmt>> &&methods_) : Stmt(StmtType::ClassStmtType),
                                                                   name(name_),
                                                                   methods(methods_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class BlockStmt : public Stmt
    {
    public:
        std::vector<std::shared_ptr<Stmt>> statements;

        BlockStmt(std::vector<std::shared_ptr<Stmt>> &&statements_) : Stmt(StmtType::BlockStmtType), statements(statements_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class IfStmt : public Stmt
    {
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> thenBranch;
        std::shared_ptr<Stmt> elseBranch;

        IfStmt(std::shared_ptr<Expr> condition_, std::shared_ptr<Stmt> thenBranch_,
               std::shared_ptr<Stmt> elseBranch_)
            : Stmt(StmtType::IfStmtType),
              condition(condition_),
              thenBranch(thenBranch_),
              elseBranch(elseBranch_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class PrintStmt : public Stmt
    {
    public:
        std::shared_ptr<Expr> expression;

        PrintStmt(std::shared_ptr<Expr> expression_)
            : Stmt(StmtType::PrintStmtType), expression(expression_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class ReturnStmt : public Stmt
    {
    public:
        TokenPtr keyword;
        std::shared_ptr<Expr> value;

        ReturnStmt(TokenPtr keyword_, std::shared_ptr<Expr> value_)
            : Stmt(StmtType::ReturnStmtType),
              keyword(keyword_),
              value(value_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class VarStmt : public Stmt
    {
    public:
        TokenPtr name;
        std::shared_ptr<Expr> initializer;

        VarStmt(TokenPtr name_, std::shared_ptr<Expr> initializer_)
            : Stmt(StmtType::VarStmtType),
              name(name_),
              initializer(initializer_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };

    class WhileStmt : public Stmt
    {
    public:
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> body;

        WhileStmt(std::shared_ptr<Expr> condition_, std::shared_ptr<Stmt> body_)
            : Stmt(StmtType::WhileStmtType),
              condition(condition_),
              body(body_) {}

        void accept(StmtVisitor &visitor) override { visitor.visit(this); }
    };
} // namespace lox

#endif