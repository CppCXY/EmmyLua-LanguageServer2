#pragma once

#include "DocBaseSyntax.h"

class DocParamSyntax : public DocBaseSyntax {
public:
    DocParamSyntax(LuaNodeOrToken n);

    std::string_view Name;

    bool Nullable = false;

    class TypeSyntax *Type = nullptr;
};
