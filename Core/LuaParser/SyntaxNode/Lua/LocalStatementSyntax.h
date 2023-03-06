#pragma once

#include "StatementSyntax.h"

class LocalStatementSyntax : public StatementSyntax {
public:
    LocalStatementSyntax(LuaSyntaxNode n);

    class NameDefSyntaxList * NameDefList = nullptr;

    class ExprSyntaxList * ExprList = nullptr;
};
