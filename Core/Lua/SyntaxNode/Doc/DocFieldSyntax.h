#pragma once

#include "DocBaseSyntax.h"
#include "Lua/Define/AccessVisibility.h"

class DocFieldSyntax : public DocBaseSyntax {
public:
    DocFieldSyntax(LuaNodeOrToken n);

    AccessVisibility Visibility = AccessVisibility::Public;

    bool Nullable = false;

    std::string_view Name;

    class FieldIndexSyntax *Index = nullptr;

    class TypeSyntax *Type = nullptr;
};
