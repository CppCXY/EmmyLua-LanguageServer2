#pragma once

#include "StatementSyntax.h"

class IfStatementSyntax : public StatementSyntax {
public:
    IfStatementSyntax(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> IfExprs;

    std::vector<class BodySyntax *> Bodys;

    class BodySyntax * ElseBody = nullptr;
};
