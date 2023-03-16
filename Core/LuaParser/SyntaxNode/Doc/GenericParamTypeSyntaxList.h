#pragma once

#include "DocBaseSyntax.h"

class GenericParamTypeSyntaxList : public DocBaseSyntax {
public:
    GenericParamTypeSyntaxList(LuaSyntaxNode n);

    std::vector<class TypeSyntax *> List;
};
