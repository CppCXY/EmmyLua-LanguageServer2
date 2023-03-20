#pragma once

#include "TypeSyntax.h"

class ArrayTypeSyntax : public TypeSyntax {
public:
    ArrayTypeSyntax(LuaNodeOrToken n);

    class TypeSyntax* Type = nullptr;
};
