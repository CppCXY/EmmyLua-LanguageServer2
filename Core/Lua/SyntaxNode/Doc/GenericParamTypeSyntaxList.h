#pragma once

#include "DocBaseSyntax.h"

class GenericParamTypeSyntaxList : public DocBaseSyntax {
public:
    GenericParamTypeSyntaxList(LuaNodeOrToken n);

    std::vector<class TypeSyntax *> List;
};
