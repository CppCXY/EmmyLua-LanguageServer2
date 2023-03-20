#pragma once

#include "TypeSyntax.h"

class FuncTypeSyntax : public TypeSyntax {
public:
    FuncTypeSyntax(LuaNodeOrToken n);

    class FuncParamTypeList *FuncParamTypeList = nullptr;

    class TypeSyntax* ReturnType = nullptr;
};
