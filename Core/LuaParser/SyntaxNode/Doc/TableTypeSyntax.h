#pragma once

#include "TypeSyntax.h"

class TableTypeSyntax : public TypeSyntax {
public:
    TableTypeSyntax(LuaSyntaxNode n);

    std::vector<class TabbleFieldSyntax *> Fields;
};
