#pragma once

#include "TypeSyntax.h"

class FuncTypeSyntax : public TypeSyntax {
public:
    FuncTypeSyntax(LuaSyntaxNode n);

    class FuncParamTypeList *FuncParamTypeList = nullptr;

    class TypeSyntax* ReturnType = nullptr;
};
