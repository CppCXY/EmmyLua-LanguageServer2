#pragma once

#include "StmtSyntax.h"

class FuncStmtSyntax : public StmtSyntax {
public:
    FuncStmtSyntax(LuaSyntaxNode n);

    class FuncNameExprSyntax *FuncName = nullptr;

    class ParamSyntaxList *ParamList = nullptr;

    class BodySyntax *Body = nullptr;
};
