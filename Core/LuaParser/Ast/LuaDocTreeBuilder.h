#pragma once

#include "LuaParser/DocParser/LuaDocParser.h"
#include "LuaSyntaxTree.h"


class LuaDocTreeBuilder {
public:
    LuaDocTreeBuilder();

    void BuildTree(LuaSyntaxTree &t, LuaDocParser &p);

private:

};


