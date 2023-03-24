#pragma once

#include "ExprSyntax.h"

class UnaryExprSyntax : public LuaBaseSyntax {
public:
    static bool CanCast(LuaSyntaxNodeKind kind) {
        return kind == LuaSyntaxNodeKind::UnaryExpression;
    }

    explicit UnaryExprSyntax(LuaNodeOrToken n = LuaNodeOrToken());

    //    LuaTokenKind UnaryOp = LuaTokenKind::TK_NOT;

    ExprSyntax GetInnerExpr(const LuaSyntaxTree &t) const;
};
