#pragma once

#include "StatementSyntax.h"

class LabelStatementSyntax : public StatementSyntax {
public:
    LabelStatementSyntax(LuaSyntaxNode n);

    std::string_view LabelName;
};


