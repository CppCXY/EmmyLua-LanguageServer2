#pragma once

#include "DocBaseSyntax.h"

enum class AccessVisibility {
    Public,
    Protected,
    Private
};

class DocAccessSyntax: public DocBaseSyntax {
public:
    DocAccessSyntax(LuaSyntaxNode n);

    AccessVisibility Visibility = AccessVisibility::Public;
};


