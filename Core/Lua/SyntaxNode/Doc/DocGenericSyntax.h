#pragma once

#include "DocBaseSyntax.h"

class DocGenericSyntax : public DocBaseSyntax {
public:
    DocGenericSyntax(LuaNodeOrToken n);

    std::vector<class GenericDeclareSyntax *> Declares;
};

