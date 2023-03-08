#pragma once

#include "StmtSyntax.h"

class FuncStmtSyntax : public StmtSyntax {
public:
    FuncStmtSyntax(LuaSyntaxNode n);

    class FunctionNameExprSyntax *FunctionName = nullptr;

    class ParamSyntaxList *ParamSyntaxList = nullptr;

    class BodySyntax *Body = nullptr;
};
