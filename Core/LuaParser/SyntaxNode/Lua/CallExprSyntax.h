#pragma once

#include "ExprSyntax.h"

class CallExprSyntax : public ExprSyntax{
public:
    CallExprSyntax(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> Args;
};

