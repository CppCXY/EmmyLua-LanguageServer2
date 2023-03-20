#pragma once

#include "LuaBaseSyntax.h"

class NameDefSyntaxList : public LuaBaseSyntax {
public:
    NameDefSyntaxList(LuaNodeOrToken n);

    std::vector<class NameDefSyntax *> List;
};

