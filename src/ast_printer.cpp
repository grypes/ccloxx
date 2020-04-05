#include "ast_printer.hpp"

using namespace lox;

void AstPrinter::visit(AssignExpr *expr)
{
    parenthesize("= " + expr->name->lexeme, expr->value.get());
}

void AstPrinter::visit(BinaryExpr *expr)
{
    parenthesize(expr->op->lexeme, expr->left.get(), expr->right.get());
}

void AstPrinter::visit(GroupingExpr *expr)
{
    parenthesize("group", expr->expression.get());
}

void AstPrinter::visit(NilLiteralExpr *expr)
{
    representation.append("Nil");
}

void AstPrinter::visit(BoolLiteralExpr *expr)
{
    representation.append(std::to_string(expr->literal));
}

void AstPrinter::visit(NumLiteralExpr *expr)
{
    representation.append(std::to_string(expr->literal));
}

void AstPrinter::visit(StrLiteralExpr *expr)
{
    representation.append(expr->literal);
}

void AstPrinter::visit(UnaryExpr *expr)
{
    parenthesize(expr->op->lexeme, expr->right.get());
}

template <typename... ExprT>
std::string AstPrinter::parenthesize(const std::string &lexeme, ExprT... exprs)
{
    std::vector<Expr *> exprvec = {exprs...};
    return parenthesize(lexeme, exprvec);
}

template <typename ExprT>
std::string AstPrinter::parenthesize(const std::string &lexeme, const std::vector<ExprT> &exprs)
{
    representation.append("(").append(lexeme);

    for (auto &expr : exprs)
    {
        representation.append(" ");
        expr->accept(*this);
    }

    representation.append(")");

    return representation;
}
