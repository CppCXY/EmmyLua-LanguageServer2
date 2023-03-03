#pragma once

#include "StatementSyntax.h"

class LocalStatementSyntax : public StatementSyntax {
public:
    using StatementSyntax::StatementSyntax;

    std::vector<class NameDefSyntax *> NameDefList;

    std::vector<class ExprSyntax *> ExprList;
};
