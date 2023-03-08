#pragma once

#include "StmtSyntax.h"

class IfStatementSyntax : public StmtSyntax {
public:
    IfStatementSyntax(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> IfExprs;

    std::vector<class BodySyntax *> Bodys;

    class BodySyntax * ElseBody = nullptr;
};
