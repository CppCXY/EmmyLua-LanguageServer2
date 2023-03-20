#pragma once

#include "StmtSyntax.h"

class CallStmtSyntax : public StmtSyntax {
public:
    CallStmtSyntax(LuaNodeOrToken n);

    class SuffixedExprSyntax *Expr = nullptr;
};
