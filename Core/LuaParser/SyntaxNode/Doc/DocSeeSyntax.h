#pragma once

#include "DocBaseSyntax.h"

class DocSeeSyntax : public DocBaseSyntax {
public:
    DocSeeSyntax(LuaSyntaxNode n);

    std::string_view Content;
};

