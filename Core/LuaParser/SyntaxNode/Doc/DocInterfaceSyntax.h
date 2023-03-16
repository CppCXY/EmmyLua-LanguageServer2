#pragma once

#include "DocBaseSyntax.h"

class DocInterfaceSyntax : public DocBaseSyntax{
public:
    DocInterfaceSyntax(LuaSyntaxNode n);

    std::string_view Name;

    class GenericDefListSyntax* GenericDefList = nullptr;
    class TypeSyntaxList* ExtendTypeList = nullptr;
};

