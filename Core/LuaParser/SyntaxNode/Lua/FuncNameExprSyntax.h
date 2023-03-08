#pragma once

#include "ExprSyntax.h"

class FunctionNameExprSyntax : public ExprSyntax {
public:
    FunctionNameExprSyntax(LuaSyntaxNode n);

    std::string_view FirstName;

    std::vector<class IndexExprSyntax *> IndexExprs;
};
