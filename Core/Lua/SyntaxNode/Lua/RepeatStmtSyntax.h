#pragma once

#include "StmtSyntax.h"

class RepeatStmtSyntax : public StmtSyntax {
public:
    RepeatStmtSyntax(LuaNodeOrToken n);

    class BodySyntax *Body = nullptr;
    class ExprSyntax *ConditionExpr = nullptr;
};
