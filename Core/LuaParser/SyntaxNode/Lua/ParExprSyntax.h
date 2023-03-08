#pragma once

#include "ExprSyntax.h"

class ParExprSyntax : public ExprSyntax {
public:
    ParExprSyntax(LuaSyntaxNode n);

    ExprSyntax *InnerExpr = nullptr;
};
