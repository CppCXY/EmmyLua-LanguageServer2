#pragma once

#include "LuaBaseSyntax.h"

class BodySyntax : public LuaBaseSyntax {
public:
    BodySyntax(LuaSyntaxNode n);

    std::vector<class StatementSyntax *> Statements;
};
