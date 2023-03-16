#pragma once

#include "DocBaseSyntax.h"

class FuncParamSyntax : public DocBaseSyntax {
public:
    FuncParamSyntax(LuaSyntaxNode n);

    std::string_view Name;
    class TypeSyntax *Type = nullptr;
};
