#pragma once

#include "ExprSyntax.h"

class IndexExprSyntax : public ExprSyntax {
public:
    IndexExprSyntax(LuaSyntaxNode n);

    LuaTokenKind AccessToken;

    class ExprSyntax *Expr = nullptr;
};
