#pragma once

#include "StatementSyntax.h"

class RepeatStatementSyntax : public StatementSyntax {
public:
    RepeatStatementSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
    class ExprSyntax *ConditionExpr = nullptr;
};
