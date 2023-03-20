#pragma once

#include "DocBaseSyntax.h"

class EnumFieldSyntax : public DocBaseSyntax {
public:
    EnumFieldSyntax(LuaNodeOrToken n);

    std::string_view Name;
};

