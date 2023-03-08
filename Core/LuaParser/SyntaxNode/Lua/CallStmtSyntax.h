#pragma once

#include "StmtSyntax.h"

class CallStmtSyntax : public StmtSyntax {
public:
    CallStmtSyntax(LuaSyntaxNode n);

    class SuffixedExprSyntax *Expr = nullptr;
};
