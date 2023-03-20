#pragma once

#include "LuaBaseSyntax.h"

class NameDefSyntax : public  LuaBaseSyntax {
public:
    NameDefSyntax(LuaNodeOrToken n);

    std::string_view Name;

    bool IsConst = false;

    bool IsClosed = false;
};


