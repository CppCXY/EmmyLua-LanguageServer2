#pragma once

#include "DocBaseSyntax.h"

class DocClassSyntax : public DocBaseSyntax {
public:
    DocClassSyntax(LuaNodeOrToken n);

    std::string_view Name;

    class GenericDefListSyntax *GenericDefList = nullptr;
    class TypeSyntaxList *ExtendTypeList = nullptr;
};
