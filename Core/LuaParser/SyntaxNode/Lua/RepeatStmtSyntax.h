#pragma once

#include "StmtSyntax.h"

class RepeatStatementSyntax : public StmtSyntax {
public:
    RepeatStatementSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
    class ExprSyntax *ConditionExpr = nullptr;
};
