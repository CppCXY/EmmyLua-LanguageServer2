#pragma once

#include "LuaBaseSyntax.h"

class ParamSyntaxList : public LuaBaseSyntax {
public:
    ParamSyntaxList(LuaNodeOrToken n);

    std::vector<std::string_view> Params;
};

