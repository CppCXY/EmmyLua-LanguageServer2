#pragma once

#include "Lua/SyntaxNode/LuaSyntaxNode.h"


class DocBaseSyntax : public LuaSyntaxNode {
public:
    DocBaseSyntax(LuaNodeOrToken n);
};
