#pragma once

#include "LuaBaseSyntax.h"

class VarSyntaxList : public LuaBaseSyntax {
public:
    VarSyntaxList(LuaSyntaxNode n);

    std::vector<class SuffixedExpressionSyntax *> Vars;
};
