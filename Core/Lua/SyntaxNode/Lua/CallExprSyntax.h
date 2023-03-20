#pragma once

#include "ExprSyntax.h"

class CallExprSyntax : public ExprSyntax{
public:
    CallExprSyntax(LuaNodeOrToken n);

    std::vector<class ExprSyntax *> Args;
};

