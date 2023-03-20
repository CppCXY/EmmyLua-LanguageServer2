#pragma once

#include "DocBaseSyntax.h"

class DocTypeSyntax : public DocBaseSyntax {
public:
    DocTypeSyntax(LuaNodeOrToken n);

    class TypeSyntaxList* TypeList = nullptr;
};
