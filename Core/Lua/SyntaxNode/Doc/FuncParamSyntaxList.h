#pragma once

#include "DocBaseSyntax.h"

class FuncParamSyntaxList : public DocBaseSyntax {
public:
    FuncParamSyntaxList(LuaNodeOrToken n);

    std::vector<class FuncParamSyntax*> FuncParams;
};

