#pragma once

#include "ExprSyntax.h"

class UnaryExprSyntax : public ExprSyntax {
public:
    UnaryExprSyntax(LuaSyntaxNode n);

    LuaTokenKind UnaryOp = LuaTokenKind::TK_NOT;

    class ExprSyntax * InnerExpr = nullptr;
};

