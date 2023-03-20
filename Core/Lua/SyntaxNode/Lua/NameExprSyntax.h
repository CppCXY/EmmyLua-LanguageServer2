#pragma once

#include "ExprSyntax.h"

class NameExprSyntax : public ExprSyntax {
public:
    NameExprSyntax(LuaNodeOrToken n);

    std::string_view Name;
};

