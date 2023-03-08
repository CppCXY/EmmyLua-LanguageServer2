#pragma once

#include "StmtSyntax.h"

class ForStmtSyntax : public StmtSyntax {
public:
    ForStmtSyntax(LuaSyntaxNode n);

    bool IsForList = false;
    bool IsForNumber = false;

    std::vector<std::string_view> Names;
    std::vector<class ExprSyntax *> Exprs;
    class BodySyntax *Body = nullptr;
};
