#pragma once

#include "TypeSyntax.h"

class IdTypeSyntax : public TypeSyntax {
public:
    IdTypeSyntax(LuaSyntaxNode n);

    std::string_view Id;
};
