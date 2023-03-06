#pragma once

#include "StatementSyntax.h"

class LocalFunctionStatementSyntax : public StatementSyntax {
public:
    LocalFunctionStatementSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class ParamSyntaxList *ParamSyntaxList = nullptr;

    class BodySyntax *Body = nullptr;
};
