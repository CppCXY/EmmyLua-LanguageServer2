#pragma once

#include "ExprSyntax.h"

class ParExprSyntax : public ExprSyntax {
public:
    ParExprSyntax(LuaNodeOrToken n);

    ExprSyntax *InnerExpr = nullptr;
};
