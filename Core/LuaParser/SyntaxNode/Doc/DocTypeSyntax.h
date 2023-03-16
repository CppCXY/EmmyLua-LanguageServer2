#pragma once

#include "DocBaseSyntax.h"

class DocTypeSyntax : public DocBaseSyntax {
public:
    DocTypeSyntax(LuaSyntaxNode n);

    class TypeSyntaxList* TypeList = nullptr;
};
