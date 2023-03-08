#pragma once

#include "StmtSyntax.h"

class ReturnStatementSyntax : public StmtSyntax {
public:
    ReturnStatementSyntax(LuaSyntaxNode n);

    class ExprSyntaxList *ReturnExprList = nullptr;
};
