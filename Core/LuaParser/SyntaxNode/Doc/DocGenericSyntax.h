#pragma once

#include "DocBaseSyntax.h"

class DocGenericSyntax : public DocBaseSyntax {
public:
    DocGenericSyntax(LuaSyntaxNode n);

    std::vector<class GenericDeclarSyntax *> Declares;
};

