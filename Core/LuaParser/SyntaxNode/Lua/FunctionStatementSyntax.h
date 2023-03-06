#pragma once

#include "StatementSyntax.h"

class FunctionStatementSyntax : public StatementSyntax {
public:
    FunctionStatementSyntax(LuaSyntaxNode n);

    class FunctionNameExprSyntax *FunctionName = nullptr;

    class ParamSyntaxList *ParamSyntaxList = nullptr;

    class BodySyntax *Body = nullptr;
};
