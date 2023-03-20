#pragma once

#include "DocBaseSyntax.h"

class DocSinceSyntax : public DocBaseSyntax {
public:
    DocSinceSyntax(LuaNodeOrToken n);

    std::string_view Content;
};
