#pragma once

#include "ExprSyntax.h"

class SuffixedExprSyntax : public ExprSyntax {
public:
    SuffixedExprSyntax(LuaNodeOrToken n);

    std::vector<class ExprSyntax *> Exprs;
};
