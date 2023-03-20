#pragma once

#include "ExprSyntax.h"

class UnaryExprSyntax : public ExprSyntax {
public:
    UnaryExprSyntax(LuaNodeOrToken n);

    LuaTokenKind UnaryOp = LuaTokenKind::TK_NOT;

    class ExprSyntax * InnerExpr = nullptr;
};

