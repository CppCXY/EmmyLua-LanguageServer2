#pragma once

#include "DocBaseSyntax.h"

class DocFieldSyntax : public DocBaseSyntax {
public:
    DocFieldSyntax(LuaSyntaxNode n);

    bool Nullable = false;

    class FieldIndexSyntax *Index = nullptr;

    class TypeSyntax *Type = nullptr;
};
