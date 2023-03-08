#pragma once

#include "ExprSyntax.h"

class TableExprSyntax : public ExprSyntax {
public:
    TableExprSyntax(LuaSyntaxNode n);

    std::vector<class TableFieldSyntax *> Fields;
};
