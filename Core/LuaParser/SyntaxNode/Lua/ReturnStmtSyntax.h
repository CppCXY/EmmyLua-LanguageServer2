#pragma once

#include "StmtSyntax.h"

class ReturnStmtSyntax : public StmtSyntax {
public:
    ReturnStmtSyntax(LuaSyntaxNode n);

    class ExprSyntaxList *ReturnExprList = nullptr;
};
