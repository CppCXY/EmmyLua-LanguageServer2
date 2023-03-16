#pragma once

#include "TypeSyntax.h"

class ParTypeSyntax : public TypeSyntax {
public:
    ParTypeSyntax(LuaSyntaxNode n);

    class TypeSyntax *Inner = nullptr;
};
