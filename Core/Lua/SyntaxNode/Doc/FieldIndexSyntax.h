#pragma once

#include "DocBaseSyntax.h"

class FieldIndexSyntax : public DocBaseSyntax {
public:
    FieldIndexSyntax(LuaNodeOrToken n);

    LuaNodeOrToken Index;
};
