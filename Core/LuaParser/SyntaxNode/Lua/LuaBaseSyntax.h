#pragma once

#include "LuaParser/SyntaxNode/BaseSyntax.h"

class LuaBaseSyntax : public BaseSyntax {
public:
    LuaBaseSyntax(LuaSyntaxNode node);

    class CommentSyntax * Comment = nullptr;
};
