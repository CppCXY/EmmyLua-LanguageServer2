#pragma once

#include "DocBaseSyntax.h"

class DocOverloadSyntax : public DocBaseSyntax{
public:
    DocOverloadSyntax(LuaSyntaxNode n);

    class FuncTypeSyntax * OverloadFunc = nullptr;
};
