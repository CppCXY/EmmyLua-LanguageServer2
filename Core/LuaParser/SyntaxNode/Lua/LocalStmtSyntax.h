#pragma once

#include "StmtSyntax.h"

class LocalStmtSyntax : public StmtSyntax {
public:
    LocalStmtSyntax(LuaSyntaxNode n);

    class NameDefSyntaxList * NameDefList = nullptr;

    class ExprSyntaxList * ExprList = nullptr;
};
