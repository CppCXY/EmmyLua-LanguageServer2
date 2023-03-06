#pragma once

#include "LuaBaseSyntax.h"

class NameDefSyntaxList : public LuaBaseSyntax {
public:
    NameDefSyntaxList(LuaSyntaxNode n);

    std::vector<class NameDefSyntax *> List;
};

