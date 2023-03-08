#pragma once

#include "ExprSyntax.h"

class ClosureExprSyntax : public ExprSyntax {
public:
    ClosureExprSyntax(LuaSyntaxNode n);

    class ParamSyntaxList * ParamList;

    class BodySyntax *Body = nullptr;
};
