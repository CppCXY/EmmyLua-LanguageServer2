#pragma once

#include "StmtSyntax.h"

class FunctionStatementSyntax : public StmtSyntax {
public:
    FunctionStatementSyntax(LuaSyntaxNode n);

    class FunctionNameExprSyntax *FunctionName = nullptr;

    class ParamSyntaxList *ParamSyntaxList = nullptr;

    class BodySyntax *Body = nullptr;
};
