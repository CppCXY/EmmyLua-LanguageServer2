#pragma once

#include "DocBaseSyntax.h"

class TypeSyntaxList : public DocBaseSyntax {
public:
    TypeSyntaxList(LuaNodeOrToken n);

    std::vector<class TypeSyntax *> TypeList;
};
