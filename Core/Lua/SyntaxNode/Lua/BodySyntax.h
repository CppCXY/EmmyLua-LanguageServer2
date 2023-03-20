#pragma once

#include "LuaBaseSyntax.h"

class BodySyntax : public LuaBaseSyntax {
public:
    BodySyntax(LuaNodeOrToken n);

    std::vector<class StmtSyntax *> Stmts;

    std::vector<class CommentSyntax *> Comments;
};
