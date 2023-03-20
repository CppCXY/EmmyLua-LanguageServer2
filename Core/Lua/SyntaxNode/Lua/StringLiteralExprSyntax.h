#pragma once

#include "ExprSyntax.h"


class StringLiteralExprSyntax : public ExprSyntax {
public:
    StringLiteralExprSyntax(LuaNodeOrToken n);

    std::string_view Content;
};
