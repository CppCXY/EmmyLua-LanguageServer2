#pragma once

#include "StmtSyntax.h"

class LocalStmtSyntax : public StmtSyntax {
public:
    LocalStmtSyntax(LuaNodeOrToken n);

    class NameDefSyntaxList * NameDefList = nullptr;

    class ExprSyntaxList * ExprList = nullptr;
};
