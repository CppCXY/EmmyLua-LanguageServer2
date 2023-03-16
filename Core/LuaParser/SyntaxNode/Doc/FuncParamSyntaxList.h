#pragma once

#include "DocBaseSyntax.h"

class FuncParamSyntaxList : public DocBaseSyntax {
public:
    FuncParamSyntaxList(LuaSyntaxNode n);

    std::vector<class FuncParamSyntax*> FuncParams;
};

