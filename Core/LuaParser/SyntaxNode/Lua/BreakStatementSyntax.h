#pragma once

#include "StatementSyntax.h"

class BreakStatementSyntax : public StatementSyntax {
public:
    BreakStatementSyntax(LuaSyntaxNode n);

    class StatementSyntax * LoopStatement = nullptr;
};
