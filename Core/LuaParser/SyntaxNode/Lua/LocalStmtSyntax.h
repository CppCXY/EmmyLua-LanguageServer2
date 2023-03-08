#pragma once

#include "StmtSyntax.h"

class LocalStatementSyntax : public StmtSyntax {
public:
    LocalStatementSyntax(LuaSyntaxNode n);

    class NameDefSyntaxList * NameDefList = nullptr;

    class ExprSyntaxList * ExprList = nullptr;
};
