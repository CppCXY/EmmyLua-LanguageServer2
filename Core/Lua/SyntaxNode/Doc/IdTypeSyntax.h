#pragma once

#include "TypeSyntax.h"

class IdTypeSyntax : public TypeSyntax {
public:
    IdTypeSyntax(LuaNodeOrToken n);

    std::string_view Id;
};
