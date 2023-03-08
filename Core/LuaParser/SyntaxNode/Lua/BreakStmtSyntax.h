#pragma once

#include "StmtSyntax.h"

class BreakStatementSyntax : public StmtSyntax {
public:
    BreakStatementSyntax(LuaSyntaxNode n);

    class StmtSyntax * LoopStatement = nullptr;
};
