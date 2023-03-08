#pragma once

#include "ExprSyntax.h"


class StringLiteralExprSyntax : public ExprSyntax {
public:
    StringLiteralExprSyntax(LuaSyntaxNode n);

    std::string_view Content;
};
