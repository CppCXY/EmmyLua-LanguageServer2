#pragma once

#include "DocBaseSyntax.h"

class DocSinceSyntax : public DocBaseSyntax {
public:
    DocSinceSyntax(LuaSyntaxNode n);

    std::string_view Content;
};
