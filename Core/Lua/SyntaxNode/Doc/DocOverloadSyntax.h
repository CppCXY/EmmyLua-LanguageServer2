#pragma once

#include "DocBaseSyntax.h"

class DocOverloadSyntax : public DocBaseSyntax{
public:
    DocOverloadSyntax(LuaNodeOrToken n);

    class FuncTypeSyntax * OverloadFunc = nullptr;
};
