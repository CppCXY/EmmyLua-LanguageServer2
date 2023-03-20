#pragma once

#include "DocBaseSyntax.h"
#include "Lua/Define/AccessVisibility.h"

class DocAccessSyntax: public DocBaseSyntax {
public:
    DocAccessSyntax(LuaNodeOrToken n);

    AccessVisibility Visibility = AccessVisibility::Public;
};


