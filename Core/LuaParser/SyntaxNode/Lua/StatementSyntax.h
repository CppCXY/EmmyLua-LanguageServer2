#pragma once

#include "LuaBaseSyntax.h"

class StatementSyntax : public LuaBaseSyntax {
public:
    StatementSyntax(LuaSyntaxNode n);

    std::vector<class CommentSyntax *> Comments;
};
