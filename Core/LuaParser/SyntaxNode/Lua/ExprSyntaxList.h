#pragma once

#include "LuaBaseSyntax.h"

class ExprSyntaxList : public LuaBaseSyntax {
public:
    ExprSyntaxList(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> List;
};
