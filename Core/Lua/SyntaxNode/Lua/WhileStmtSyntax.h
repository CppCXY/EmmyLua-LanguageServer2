#pragma once

#include "StmtSyntax.h"

class WhileStmtSyntax : public StmtSyntax {
public:
    WhileStmtSyntax(LuaNodeOrToken n);

    class ExprSyntax *ConditionExpr = nullptr;

    class BodySyntax *Body = nullptr;
};
