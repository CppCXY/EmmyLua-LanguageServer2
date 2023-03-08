#pragma once

#include "StmtSyntax.h"

class DoStatementSyntax : public StmtSyntax {
public:
    DoStatementSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
};
