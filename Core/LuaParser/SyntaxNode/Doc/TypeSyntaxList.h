#pragma once

#include "DocBaseSyntax.h"

class TypeSyntaxList : public DocBaseSyntax {
public:
    TypeSyntaxList(LuaSyntaxNode n);

    std::vector<class TypeSyntax *> TypeList;
};
