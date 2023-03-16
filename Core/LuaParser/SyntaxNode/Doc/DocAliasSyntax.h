#pragma once

#include "DocBaseSyntax.h"

class DocAliasSyntax : public DocBaseSyntax {
public:
    DocAliasSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class TypeSyntax *Type = nullptr;
};
