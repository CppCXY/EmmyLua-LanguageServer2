#pragma once

#include "TypeSyntax.h"

class ParTypeSyntax : public TypeSyntax {
public:
    ParTypeSyntax(LuaNodeOrToken n);

    class TypeSyntax *Inner = nullptr;
};
