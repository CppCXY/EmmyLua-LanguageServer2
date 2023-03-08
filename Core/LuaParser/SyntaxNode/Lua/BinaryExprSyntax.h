#pragma once

#include "ExprSyntax.h"

class BinaryExprSyntax : public ExprSyntax {
public:
    BinaryExprSyntax(LuaSyntaxNode n);

    LuaTokenKind BinaryOp = LuaTokenKind::TK_PLUS;

    class ExprSyntax *LeftExpr = nullptr;
    class ExprSyntax *RightExpr = nullptr;
};
