#pragma once

#include "LuaBaseSyntax.h"

class ExprSyntax : public LuaBaseSyntax {
public:
    ExprSyntax(LuaNodeOrToken n);
};
