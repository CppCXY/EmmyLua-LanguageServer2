#pragma once

#include "TypeSyntax.h"

class ArrayTypeSyntax : public TypeSyntax {
public:
    ArrayTypeSyntax(LuaSyntaxNode n);

    class TypeSyntax* Type = nullptr;
};
