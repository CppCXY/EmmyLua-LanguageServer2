#pragma once

#include "StmtSyntax.h"

class AssignStmtSyntax : public StmtSyntax {
public:
    AssignStmtSyntax(LuaNodeOrToken n);

    class VarSyntaxList *VarList = nullptr;
    class ExprSyntaxList *ExprList = nullptr;
};
