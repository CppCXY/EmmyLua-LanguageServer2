#pragma once

#include "DocBaseSyntax.h"

class DocSeeSyntax : public DocBaseSyntax {
public:
    DocSeeSyntax(LuaNodeOrToken n);

    std::string_view Content;
};

