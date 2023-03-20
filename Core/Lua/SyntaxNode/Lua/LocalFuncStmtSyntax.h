#pragma once

#include "StmtSyntax.h"

class LocalFuncStmtSyntax : public StmtSyntax {
public:
    LocalFuncStmtSyntax(LuaNodeOrToken n);

    std::string_view Name;

    class ParamSyntaxList *ParamList = nullptr;

    class BodySyntax *Body = nullptr;
};
