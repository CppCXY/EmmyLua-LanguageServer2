#pragma once

#include "StatementSyntax.h"

class ReturnStatementSyntax : public StatementSyntax {
public:
    ReturnStatementSyntax(LuaSyntaxNode n);

    class ExprSyntaxList *ReturnExprList = nullptr;
};
