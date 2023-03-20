#pragma once

#include "ExprSyntax.h"

class LiteralExprSyntax : public ExprSyntax {
public:
    LiteralExprSyntax(LuaNodeOrToken n);

    std::string_view Literal;
};
