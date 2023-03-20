#pragma once

#include "Lua/SyntaxNode/BaseSyntax.h"

class LuaBaseSyntax : public BaseSyntax {
public:
    LuaBaseSyntax(LuaNodeOrToken node);

    class CommentSyntax * Comment = nullptr;
};
