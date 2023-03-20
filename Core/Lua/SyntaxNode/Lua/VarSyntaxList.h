#pragma once

#include "LuaBaseSyntax.h"

class VarSyntaxList : public LuaBaseSyntax {
public:
    VarSyntaxList(LuaNodeOrToken n);

    std::vector<class ExprSyntax *> Vars;
};
