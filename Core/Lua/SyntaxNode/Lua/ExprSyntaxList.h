#pragma once

#include "LuaBaseSyntax.h"

class ExprSyntaxList : public LuaBaseSyntax {
public:
    ExprSyntaxList(LuaNodeOrToken n);

    std::vector<class ExprSyntax *> List;
};
