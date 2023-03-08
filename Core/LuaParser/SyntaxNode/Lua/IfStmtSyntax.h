#pragma once

#include "StmtSyntax.h"

class IfStmtSyntax : public StmtSyntax {
public:
    IfStmtSyntax(LuaSyntaxNode n);

    std::vector<class ExprSyntax *> IfExprs;

    std::vector<class BodySyntax *> Bodys;

    class BodySyntax * ElseBody = nullptr;
};
