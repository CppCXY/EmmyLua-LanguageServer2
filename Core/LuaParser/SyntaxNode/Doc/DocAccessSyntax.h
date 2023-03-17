#pragma once

#include "DocBaseSyntax.h"
#include "LuaParser/Define/AccessVisibility.h"

class DocAccessSyntax: public DocBaseSyntax {
public:
    DocAccessSyntax(LuaSyntaxNode n);

    AccessVisibility Visibility = AccessVisibility::Public;
};


