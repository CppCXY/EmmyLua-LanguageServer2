#pragma once

#include "StmtSyntax.h"

class LabelStatementSyntax : public StmtSyntax {
public:
    LabelStatementSyntax(LuaSyntaxNode n);

    std::string_view LabelName;
};


