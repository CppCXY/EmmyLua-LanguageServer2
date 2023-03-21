#pragma once

#include "DocBaseSyntax.h"

class DocInterfaceSyntax : public DocBaseSyntax{
public:
    DocInterfaceSyntax(LuaNodeOrToken n);

    std::string_view Name;

    class GenericDefListSyntax* GenericDefList = nullptr;
    class TypeSyntaxList* ExtendTypeList = nullptr;
};
