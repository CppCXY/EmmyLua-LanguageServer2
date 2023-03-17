#pragma once

#include "DocBaseSyntax.h"

class FieldIndexSyntax : public DocBaseSyntax {
public:
    FieldIndexSyntax(LuaSyntaxNode n);

    LuaSyntaxNode Index;
};
