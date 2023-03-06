#pragma once

#include "StatementSyntax.h"

class ForStatementSyntax : public StatementSyntax {
public:
    ForStatementSyntax(LuaSyntaxNode n);

    bool IsForList = false;
    bool IsForNumber = false;

    std::vector<std::string_view> Names;
    std::vector<class ExprSyntax *> Exprs;
    class BodySyntax *Body = nullptr;
};
