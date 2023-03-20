#pragma once

#include "StmtSyntax.h"

class BreakStmtSyntax : public StmtSyntax {
public:
    BreakStmtSyntax(LuaNodeOrToken n);

    class StmtSyntax * LoopStatement = nullptr;
};
