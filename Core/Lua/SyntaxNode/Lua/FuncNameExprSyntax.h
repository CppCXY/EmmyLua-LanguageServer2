#pragma once

#include "ExprSyntax.h"

class FuncNameExprSyntax : public ExprSyntax {
public:
    FuncNameExprSyntax(LuaNodeOrToken n);

    std::string_view FirstName;

    std::vector<class IndexExprSyntax *> IndexExprs;
};
