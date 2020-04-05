#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <vector>

#include "scanner.hpp"

namespace lox
{

class AssignExpr;
class BinaryExpr;
// class CallExpr;
class GetExpr;
class GroupingExpr;
class BoolLiteralExpr;
class NilLiteralExpr;
class NumLiteralExpr;
class StrLiteralExpr;
class LogicExpr;
class SetExpr;
class SuperExpr;
class ThisExpr;
class UnaryExpr;
class VarExpr;

class ExprVisitor
{
public:
    virtual void visit(AssignExpr *expr) = 0;
    virtual void visit(BinaryExpr *expr) = 0;
    // virtual void visit(CallExpr *expr) = 0;
    // virtual void visit(GetExpr *expr) = 0;
    virtual void visit(GroupingExpr *expr) = 0;
    virtual void visit(NilLiteralExpr *expr) = 0;
    virtual void visit(BoolLiteralExpr *expr) = 0;
    virtual void visit(NumLiteralExpr *expr) = 0;
    virtual void visit(StrLiteralExpr *expr) = 0;
    virtual void visit(LogicExpr *expr) = 0;
    // virtual void visit(SetExpr *expr) = 0;
    // virtual void visit(SuperExpr *expr) = 0;
    // virtual void visit(ThisExpr *expr) = 0;
    virtual void visit(VarExpr *expr) = 0;
    virtual void visit(UnaryExpr *expr) = 0;
};

enum class ExprType
{
    AssignExprType,
    BinaryExprType,
    CallExprType,
    GetExprType,
    GroupExprType,
    NilLiteralExprType,
    BoolLiteralExprType,
    NumLiteralExprType,
    StrLiteralExprType,
    LogicalExprType,
    setExprType,
    SuperExprType,
    ThisExprType,
    UnaryExprType,
    VarExprType
};

class Expr
{
public:
    ExprType type;

    Expr(ExprType type_) : type(type_) {}

    virtual ~Expr() {}

    virtual std::unique_ptr<Expr> clone() = 0;

    virtual void accept(ExprVisitor &visitor) = 0;
};

class AssignExpr : public Expr
{
public:
    std::unique_ptr<Token> name;
    std::unique_ptr<Expr> value;

    AssignExpr(std::unique_ptr<Token> name_, std::unique_ptr<Expr> value_) : Expr(ExprType::AssignExprType),
                                                                             name(std::move(name_)),
                                                                             value(std::move(value_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<AssignExpr>(new AssignExpr(name->clone(), value->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BinaryExpr : public Expr
{
public:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Token> op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left_, std::unique_ptr<Token> op_,
               std::unique_ptr<Expr> right_) : Expr(ExprType::BinaryExprType),
                                               left(std::move(left_)),
                                               op(std::move(op_)),
                                               right(std::move(right_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<BinaryExpr>(new BinaryExpr(left->clone(), op->clone(), right->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

// class CallExpr : public Expr
// {
// public:
//     std::unique_ptr<Expr> callee;
//     std::unique_ptr<Token> paren;
//     std::vector<std::unique_ptr<Expr>> arguments;

//     CallExpr(std::unique_ptr<Expr> callee_, std::unique_ptr<Token> paren_,
//              std::vector<std::unique_ptr<Expr>> &arguments_) : Expr(ExprType::CallExprType),
//                                                                callee(std::move(callee_)),
//                                                                paren(std::move(paren_))
//     {
//         for (auto &a : arguments_)
//             arguments.push_back(std::move(a));
//     }

//     std::unique_ptr<Expr> clone() override
//     {
//         std::vector<std::unique_ptr<Expr>> new_arguments;
//         for (auto &a : arguments)
//         {
//             new_arguments.push_back(a->clone());
//         }
//         return std::unique_ptr<CallExpr>(new CallExpr(callee->clone(), paren->clone(), std::move(new_arguments)));
//     }

//     void accept(ExprVisitor &visitor) override { visitor.visit(this); }
// };

// class GetExpr : public Expr
// {
// public:
//     std::unique_ptr<Expr> obj;
//     std::unique_ptr<Token> name;

//     GetExpr(std::unique_ptr<Expr> obj_, std::unique_ptr<Token> name_) : Expr(ExprType::GetExprType),
//                                                                         obj(std::move(obj)),
//                                                                         name(std::move(name_)) {}

//     std::unique_ptr<Expr> clone() override
//     {
//         return std::unique_ptr<GetExpr>(new GetExpr(obj->clone(), name->clone()));
//     }

//     void accept(ExprVisitor &visitor) override { visitor.visit(this); }
// };

class GroupingExpr : public Expr
{
public:
    std::unique_ptr<Expr> expression;

    GroupingExpr(std::unique_ptr<Expr> expression_) : Expr(ExprType::GroupExprType),
                                                      expression(std::move(expression_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<GroupingExpr>(new GroupingExpr(expression->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NilLiteralExpr : public Expr
{
public:
    NilLiteralExpr() : Expr(ExprType::NilLiteralExprType) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<NilLiteralExpr>(new NilLiteralExpr());
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class BoolLiteralExpr : public Expr
{
public:
    bool literal;

    BoolLiteralExpr(const bool literal_) : Expr(ExprType::BoolLiteralExprType), literal(literal_) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<BoolLiteralExpr>(new BoolLiteralExpr(literal));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class NumLiteralExpr : public Expr
{
public:
    double literal;

    NumLiteralExpr(const double literal_) : Expr(ExprType::NumLiteralExprType), literal(literal_) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<NumLiteralExpr>(new NumLiteralExpr(literal));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class StrLiteralExpr : public Expr
{
public:
    std::string literal;

    StrLiteralExpr(const std::string literal_) : Expr(ExprType::StrLiteralExprType), literal(literal_) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<StrLiteralExpr>(new StrLiteralExpr(literal));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class LogicExpr : public Expr
{
public:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Token> opr;
    std::unique_ptr<Expr> right;

    LogicExpr(std::unique_ptr<Expr> left_, std::unique_ptr<Token> opr_, std::unique_ptr<Expr> right_) : Expr(ExprType::LogicalExprType),
                                                                                                        left(std::move(left_)), opr(std::move(opr_)), right(std::move(right_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<LogicExpr>(new LogicExpr(left->clone(), opr->clone(), right->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

// class SetExpr : public Expr
// {
// public:
//     std::unique_ptr<Expr> obj;
//     std::unique_ptr<Token> name;
//     std::unique_ptr<Expr> value;

//     SetExpr(std::unique_ptr<Expr> obj_, std::unique_ptr<Token> name_, std::unique_ptr<Expr> value_) : Expr(ExprType::setExprType),
//                                                                                                       obj(std::move(obj_)), name(std::move(name_)), value(std::move(value_)) {}

//     std::unique_ptr<Expr> clone() override
//     {
//         return std::unique_ptr<SetExpr>(new SetExpr(obj->clone(), name->clone(), value->clone()));
//     }

//     void accept(ExprVisitor &visitor) override { visitor.visit(this); }
// };

// class SuperExpr : public Expr
// {
// public:
//     std::unique_ptr<Token> keyword;
//     std::unique_ptr<Token> method;

//     SuperExpr(std::unique_ptr<Token> keyword_, std::unique_ptr<Token> method_) : Expr(ExprType::SuperExprType),
//                                                                                  keyword(std::move(keyword_)), method(std::move(method_)) {}

//     std::unique_ptr<Expr> clone() override
//     {
//         return std::unique_ptr<SuperExpr>(new SuperExpr(keyword->clone(), method->clone()));
//     }

//     void accept(ExprVisitor &visitor) override { visitor.visit(this); }
// };

// class ThisExpr : public Expr
// {
// public:
//     std::unique_ptr<Token> keyword;

//     ThisExpr(std::unique_ptr<Token> keyword_) : Expr(ExprType::ThisExprType),
//                                                 keyword(std::move(keyword_)) {}

//     std::unique_ptr<Expr> clone() override
//     {
//         return std::unique_ptr<ThisExpr>(new ThisExpr(keyword->clone()));
//     }

//     void accept(ExprVisitor &visitor) override { visitor.visit(this); }
// };

class VarExpr : public Expr
{
public:
    std::unique_ptr<Token> name;

    VarExpr(std::unique_ptr<Token> name_) : Expr(ExprType::VarExprType),
                                            name(std::move(name_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<VarExpr>(new VarExpr(name->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

class UnaryExpr : public Expr
{
public:
    std::unique_ptr<Token> op;
    std::unique_ptr<Expr> right;

    UnaryExpr(std::unique_ptr<Token> op_, std::unique_ptr<Expr> right_) : Expr(ExprType::UnaryExprType),
                                                                          op(std::move(op_)),
                                                                          right(std::move(right_)) {}

    std::unique_ptr<Expr> clone() override
    {
        return std::unique_ptr<UnaryExpr>(new UnaryExpr(op->clone(), right->clone()));
    }

    void accept(ExprVisitor &visitor) override { visitor.visit(this); }
};

/*****************************************/

class BlockStmt;
class ClassStmt;
class ExprStmt;
class FuncStmt;
class IfStmt;
class PrintStmt;
class ReturnStmt;
class VarStmt;
class WhileStmt;

class StmtVisitor
{
public:
    virtual void visit(BlockStmt *stmt) = 0;
    // virtual void visit(ClassStmt *stmt) = 0;
    virtual void visit(ExprStmt *stmt) = 0;
    // virtual void visit(FuncStmt *stmt) = 0;
    virtual void visit(IfStmt *stmt) = 0;
    virtual void visit(PrintStmt *stmt) = 0;
    // virtual void visit(ReturnStmt *stmt) = 0;
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

    virtual std::unique_ptr<Stmt> clone() = 0;

    virtual void accept(StmtVisitor &visitor) = 0;
};

class ExprStmt : public Stmt
{
public:
    std::unique_ptr<Expr> expression;

    ExprStmt(std::unique_ptr<Expr> expression_) : Stmt(StmtType::ExprStmtType),
                                                  expression(std::move(expression_)) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<ExprStmt>(new ExprStmt(expression->clone()));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

// class ClassStmt : public Stmt
// {
// public:
//     std::unique_ptr<Token> name;
//     std::unique_ptr<Expr> superclass;
//     std::vector<std::unique_ptr<FuncStmt>> methods;

//     ClassStmt(std::unique_ptr<Token> name_, std::unique_ptr<Expr> superclass_,
//               std::vector<std::unique_ptr<FuncStmt>> &methods_) : Stmt(StmtType::ClassStmtType),
//                                                                   name(std::move(name_)),
//                                                                   superclass(std::move(superclass_))
//     {
//         for (auto &m : methods_)
//             methods.push_back(std::move(m));
//     }

//     std::unique_ptr<Stmt> clone() override
//     {
//         std::vector<std::unique_ptr<FuncStmt>> new_methods;
//         for (auto &m : methods)
//             new_methods.push_back(std::move(m));
//         return std::unique_ptr<ClassStmt>(new ClassStmt(name->clone(), superclass->clone(), std::move(new_methods)));
//     }

//     void accept(StmtVisitor &visitor) override { visitor.visit(this); }
// };

// class FuncStmt : public Stmt
// {
// public:
//     std::unique_ptr<Token> name;
//     std::vector<std::unique_ptr<Token>> params;
//     std::vector<std::shared_ptr<Stmt>> body;

//     FuncStmt(std::unique_ptr<Token> name_,
//              std::vector<std::unique_ptr<Token>> &params_,
//              std::vector<std::shared_ptr<Stmt>> &body_) : Stmt(StmtType::FuncStmtType),
//                                                           name(std::move(name_)),
//                                                           body(body_)
//     {
//         for (auto &p : params_)
//             params.push_back(std::move(p));
//     }

//     std::unique_ptr<Stmt> clone() override
//     {
//         std::vector<std::unique_ptr<Token>> new_params;

//         for (auto &p : params)
//             new_params.push_back(p->clone());
//         return std::unique_ptr<FuncStmt>(new FuncStmt(name->clone(), new_params, body));
//     }

//     void accept(StmtVisitor &visitor) override { visitor.visit(this); }
// };

class BlockStmt : public Stmt
{
public:
    std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::shared_ptr<Stmt>> &statements_) : Stmt(StmtType::BlockStmtType), statements(statements_) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<BlockStmt>(new BlockStmt(statements));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class IfStmt : public Stmt
{
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> condition_, std::unique_ptr<Stmt> thenBranch_,
           std::unique_ptr<Stmt> elseBranch_)
        : Stmt(StmtType::IfStmtType),
          condition(std::move(condition_)),
          thenBranch(std::move(thenBranch_)),
          elseBranch(std::move(elseBranch_)) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<IfStmt>(new IfStmt(condition->clone(), thenBranch->clone(),
                                                  elseBranch->clone()));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class PrintStmt : public Stmt
{
public:
    std::unique_ptr<Expr> expression;

    PrintStmt(std::unique_ptr<Expr> expression_)
        : Stmt(StmtType::PrintStmtType), expression(std::move(expression_)) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<PrintStmt>(new PrintStmt(expression->clone()));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

// class ReturnStmt : public Stmt
// {
// public:
//     std::unique_ptr<Token> keyword;
//     std::unique_ptr<Expr> value;

//     ReturnStmt(std::unique_ptr<Token> keyword_, std::unique_ptr<Expr> value_)
//         : Stmt(StmtType::ReturnStmtType),
//           keyword(std::move(keyword_)),
//           value(std::move(value_)) {}

//     std::unique_ptr<Stmt> clone() override
//     {
//         return std::unique_ptr<ReturnStmt>(new ReturnStmt(keyword->clone(), value->clone()));
//     }

//     void accept(StmtVisitor &visitor) override { visitor.visit(this); }
// };

class VarStmt : public Stmt
{
public:
    std::unique_ptr<Token> name;
    std::unique_ptr<Expr> initializer;

    VarStmt(std::unique_ptr<Token> name_, std::unique_ptr<Expr> initializer_)
        : Stmt(StmtType::VarStmtType),
          name(std::move(name_)),
          initializer(std::move(initializer_)) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<VarStmt>(new VarStmt(name->clone(), initializer->clone()));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};

class WhileStmt : public Stmt
{
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition_, std::unique_ptr<Stmt> body_)
        : Stmt(StmtType::WhileStmtType),
          condition(std::move(condition_)),
          body(std::move(body_)) {}

    std::unique_ptr<Stmt> clone() override
    {
        return std::unique_ptr<WhileStmt>(new WhileStmt(condition->clone(), body->clone()));
    }

    void accept(StmtVisitor &visitor) override { visitor.visit(this); }
};
} // namespace lox

#endif