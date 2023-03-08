#pragma once

#include "StmtSyntax.h"

class RepeatStmtSyntax : public StmtSyntax {
public:
    RepeatStmtSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
    class ExprSyntax *ConditionExpr = nullptr;
};
