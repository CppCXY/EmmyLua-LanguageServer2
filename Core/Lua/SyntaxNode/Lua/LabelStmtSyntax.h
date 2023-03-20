#pragma once

#include "StmtSyntax.h"

class LabelStmtSyntax : public StmtSyntax {
public:
    LabelStmtSyntax(LuaNodeOrToken n);

    std::string_view LabelName;
};


