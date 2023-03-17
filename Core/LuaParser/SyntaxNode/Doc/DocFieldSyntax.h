#pragma once

#include "DocBaseSyntax.h"
#include "LuaParser/Define/AccessVisibility.h"

class DocFieldSyntax : public DocBaseSyntax {
public:
    DocFieldSyntax(LuaSyntaxNode n);

    AccessVisibility Visibility = AccessVisibility::Public;

    bool Nullable = false;

    std::string_view Name;

    class FieldIndexSyntax *Index = nullptr;

    class TypeSyntax *Type = nullptr;
};
