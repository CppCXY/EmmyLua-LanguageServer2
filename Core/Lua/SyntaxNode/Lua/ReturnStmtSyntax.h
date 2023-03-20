#pragma once

#include "StmtSyntax.h"

class ReturnStmtSyntax : public StmtSyntax {
public:
    ReturnStmtSyntax(LuaNodeOrToken n);

    class ExprSyntaxList *ReturnExprList = nullptr;
};
