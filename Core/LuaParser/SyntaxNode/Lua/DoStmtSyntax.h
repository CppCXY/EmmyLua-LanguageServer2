#pragma once

#include "StmtSyntax.h"

class DoStmtSyntax : public StmtSyntax {
public:
    DoStmtSyntax(LuaSyntaxNode n);

    class BodySyntax *Body = nullptr;
};
