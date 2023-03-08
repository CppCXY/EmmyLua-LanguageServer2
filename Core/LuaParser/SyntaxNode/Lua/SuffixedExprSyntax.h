#pragma once

#include "ExprSyntax.h"

class SuffixedExprSyntax : public ExprSyntax {
public:
    SuffixedExprSyntax(LuaSyntaxNode n);

    class ExprSyntax *PrimaryExpr = nullptr;

    std::vector<class IndexExprSyntax *> IndexExprs;

    class CallSyntax *CallExpr = nullptr;
};
