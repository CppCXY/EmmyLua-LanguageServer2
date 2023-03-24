#pragma once

#include "LuaBaseSyntax.h"
#include "StmtSyntax.h"

class BodySyntax : public LuaBaseSyntax {
public:
    static bool CanCast(LuaSyntaxNodeKind kind) {
        return kind == LuaSyntaxNodeKind::Body;
    }

    explicit BodySyntax(LuaNodeOrToken n = LuaNodeOrToken());

    std::vector<StmtSyntax> GetStmts(const LuaSyntaxTree& t);

    std::vector<CommentSyntax> GetComments(const LuaSyntaxTree& t);
};
