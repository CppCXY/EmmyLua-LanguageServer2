#pragma once

#include "TypeSyntax.h"

class StringTypeSyntax : public TypeSyntax {
public:
    StringTypeSyntax(LuaNodeOrToken n);
    std::string_view InnerText;
};
