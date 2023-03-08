#pragma once

#include "StmtSyntax.h"

class WhileStatementSyntax : public StmtSyntax {
public:
    WhileStatementSyntax(LuaSyntaxNode n);

    class ExprSyntax *ConditionExpr = nullptr;

    class BodySyntax *Body = nullptr;
};
