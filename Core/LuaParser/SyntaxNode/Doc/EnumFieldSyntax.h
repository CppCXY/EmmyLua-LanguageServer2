#pragma once

#include "DocBaseSyntax.h"

class EnumFieldSyntax : public DocBaseSyntax {
public:
    EnumFieldSyntax(LuaSyntaxNode n);

    std::string_view Name;
};

