#pragma once

#include "StatementSyntax.h"

class DoStatementSyntax : public StatementSyntax {
public:
    DoStatementSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
};
