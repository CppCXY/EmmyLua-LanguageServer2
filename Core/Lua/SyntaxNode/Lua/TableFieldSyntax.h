#pragma once

#include "LuaBaseSyntax.h"

class TableFieldSyntax : public LuaBaseSyntax {
public:
    TableFieldSyntax(LuaNodeOrToken n);

    bool IsKeyValue = false;

    class ExprSyntax *Key = nullptr;
    class ExprSyntax *Value = nullptr;
};
