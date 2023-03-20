#pragma once

#include "ExprSyntax.h"

class TableExprSyntax : public ExprSyntax {
public:
    TableExprSyntax(LuaNodeOrToken n);

    std::vector<class TableFieldSyntax *> Fields;
};
