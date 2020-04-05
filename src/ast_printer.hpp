#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "ast.hpp"

namespace lox
{
class AstPrinter : public ExprVisitor
{
public:
    std::string representation;

    AstPrinter() {}

    void visit(AssignExpr *expr) override;
    void visit(BinaryExpr *expr) override;
    void visit(GroupingExpr *expr) override;
    void visit(NilLiteralExpr *expr) override;
    void visit(BoolLiteralExpr *expr) override;
    void visit(StrLiteralExpr *expr) override;
    void visit(NumLiteralExpr *expr) override;
    void visit(UnaryExpr *expr) override;

private:
    template <typename... ExprT>
    std::string parenthesize(const std::string &lexeme, ExprT... exprs);

    template <typename ExprT>
    std::string parenthesize(const std::string &lexeme, const std::vector<ExprT> &exprs);
};
} // namespace lox

#endif