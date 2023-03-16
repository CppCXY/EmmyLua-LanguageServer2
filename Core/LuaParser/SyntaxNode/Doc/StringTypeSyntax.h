#pragma once

#include "TypeSyntax.h"

class StringTypeSyntax : public TypeSyntax {
public:
    StringTypeSyntax(LuaSyntaxNode n);
    std::string_view InnerText;
};
