#pragma once

#include "StmtSyntax.h"

class BreakStmtSyntax : public StmtSyntax {
public:
    BreakStmtSyntax(LuaSyntaxNode n);

    class StmtSyntax * LoopStatement = nullptr;
};
