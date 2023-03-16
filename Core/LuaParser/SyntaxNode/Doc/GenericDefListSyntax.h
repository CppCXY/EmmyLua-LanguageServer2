#pragma once

#include "DocBaseSyntax.h"

class GenericDefListSyntax: public DocBaseSyntax {
public:
    GenericDefListSyntax(LuaSyntaxNode n);

    std::vector<std::string_view> TypeNameList;
};

