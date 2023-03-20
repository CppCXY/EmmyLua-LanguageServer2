#pragma once

#include "ExprSyntax.h"

class IndexExprSyntax : public ExprSyntax {
public:
    IndexExprSyntax(LuaNodeOrToken n);

    LuaTokenKind AccessToken;

    class ExprSyntax *Expr = nullptr;
};
