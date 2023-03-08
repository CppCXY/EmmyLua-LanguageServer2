#pragma once

#include "ExprSyntax.h"

class SuffixedExprSyntax : public ExprSyntax {
public:
    SuffixedExprSyntax(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> Exprs;
};
