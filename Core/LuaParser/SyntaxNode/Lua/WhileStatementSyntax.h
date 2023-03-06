#pragma once

#include "StatementSyntax.h"

class WhileStatementSyntax : public StatementSyntax {
public:
    WhileStatementSyntax(LuaSyntaxNode n);

    class ExprSyntax *ConditionExpr = nullptr;

    class BodySyntax *Body = nullptr;
};
