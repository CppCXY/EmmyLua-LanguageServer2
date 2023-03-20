#pragma once

#include "StmtSyntax.h"

class DoStmtSyntax : public StmtSyntax {
public:
    DoStmtSyntax(LuaNodeOrToken n);

    class BodySyntax *Body = nullptr;
};
