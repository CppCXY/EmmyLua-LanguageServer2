#pragma once

#include "DocBaseSyntax.h"

class DocEnumSyntax : public DocBaseSyntax {
public:
    DocEnumSyntax(LuaNodeOrToken n);

    std::string_view Name;

    class TypeSyntax *SuperType = nullptr;

    std::vector<class EnumFieldSyntax *> Fields;
};
