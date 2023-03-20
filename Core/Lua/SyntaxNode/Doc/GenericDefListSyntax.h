#pragma once

#include "DocBaseSyntax.h"

class GenericDefListSyntax: public DocBaseSyntax {
public:
    GenericDefListSyntax(LuaNodeOrToken n);

    std::vector<std::string_view> TypeNameList;
};

