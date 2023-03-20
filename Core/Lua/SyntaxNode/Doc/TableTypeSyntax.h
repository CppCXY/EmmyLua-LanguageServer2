#pragma once

#include "TypeSyntax.h"

class TableTypeSyntax : public TypeSyntax {
public:
    TableTypeSyntax(LuaNodeOrToken n);

    std::vector<class TableFieldTypeSyntax *> Fields;
};
