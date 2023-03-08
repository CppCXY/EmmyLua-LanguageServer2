#pragma once

#include "StmtSyntax.h"

class GotoStmtSyntax : public StmtSyntax {
public:
    GotoStmtSyntax(LuaSyntaxNode n);

    std::string_view Label;
};

