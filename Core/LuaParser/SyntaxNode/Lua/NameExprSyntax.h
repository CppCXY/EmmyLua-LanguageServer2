#pragma once

#include "ExprSyntax.h"

class NameExprSyntax : public ExprSyntax {
public:
    NameExprSyntax(LuaSyntaxNode n);

    std::string_view Name;
};

