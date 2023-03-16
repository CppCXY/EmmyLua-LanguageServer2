#pragma once

#include "DocBaseSyntax.h"

class DocParamSyntax : public DocBaseSyntax {
public:
    DocParamSyntax(LuaSyntaxNode n);

    std::string_view Name;

    bool Nullable = false;

    class TypeSyntax *Type = nullptr;
};
