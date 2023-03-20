#pragma once

#include "StmtSyntax.h"

class GotoStmtSyntax : public StmtSyntax {
public:
    GotoStmtSyntax(LuaNodeOrToken n);

    std::string_view Label;
};

