#pragma once

#include "StmtSyntax.h"

class LabelStmtSyntax : public StmtSyntax {
public:
    LabelStmtSyntax(LuaSyntaxNode n);

    std::string_view LabelName;
};


