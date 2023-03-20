#pragma once

#include "TypeSyntax.h"

class GenericTypeSyntax : public TypeSyntax {
public:
    GenericTypeSyntax(LuaNodeOrToken n);

    std::string_view BaseName;

    class GenericParamTypeSyntaxList *GenericList = nullptr;
};
