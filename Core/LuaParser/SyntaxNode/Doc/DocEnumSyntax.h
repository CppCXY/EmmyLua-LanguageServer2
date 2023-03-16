#pragma once

#include "DocBaseSyntax.h"

class DocEnumSyntax : public DocBaseSyntax {
public:
    DocEnumSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class TypeSyntax *SuperType = nullptr;

    std::vector<class EnumFieldSyntax *> Fields;
};
