#pragma once

#include "Lua/SyntaxNode/BaseSyntax.h"


class DocBaseSyntax : public BaseSyntax {
public:
    DocBaseSyntax(LuaNodeOrToken n);
};
