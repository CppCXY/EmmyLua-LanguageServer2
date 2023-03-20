#pragma once

#include "TypeSyntax.h"

class UnionTypeSyntax : public TypeSyntax {
public:
    UnionTypeSyntax(LuaNodeOrToken n);

    class TypeSyntax *Left = nullptr;
    class TypeSyntax *Right = nullptr;
};
