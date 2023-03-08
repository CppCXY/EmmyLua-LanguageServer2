#pragma once

#include "StmtSyntax.h"

class LocalFunctionStatementSyntax : public StmtSyntax {
public:
    LocalFunctionStatementSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class ParamSyntaxList *ParamSyntaxList = nullptr;

    class BodySyntax *Body = nullptr;
};
