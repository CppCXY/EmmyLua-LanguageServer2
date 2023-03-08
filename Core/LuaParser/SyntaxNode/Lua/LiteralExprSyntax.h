#pragma once

#include "ExprSyntax.h"

class LiteralExprSyntax : public ExprSyntax {
public:
    LiteralExprSyntax(LuaSyntaxNode n);

    std::string_view Literal;
};
