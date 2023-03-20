#pragma once

#include "StmtSyntax.h"

class IfStmtSyntax : public StmtSyntax {
public:
    IfStmtSyntax(LuaNodeOrToken n);

    std::vector<class ExprSyntax *> IfExprs;

    std::vector<class BodySyntax *> Bodys;

    class BodySyntax * ElseBody = nullptr;
};
