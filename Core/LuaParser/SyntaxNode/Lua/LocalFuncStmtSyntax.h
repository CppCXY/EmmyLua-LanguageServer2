#pragma once

#include "StmtSyntax.h"

class LocalFuncStmtSyntax : public StmtSyntax {
public:
    LocalFuncStmtSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class ParamSyntaxList *ParamList = nullptr;

    class BodySyntax *Body = nullptr;
};
