#pragma once

#include "Lua/DocParser/LuaDocParser.h"
#include "LuaSyntaxTree.h"


class LuaDocTreeBuilder {
public:
    LuaDocTreeBuilder();

    void BuildTree(LuaSyntaxTree &t, LuaDocParser &p, std::size_t parent);

private:
    void StartNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaDocParser &p);

    void EatToken(LuaSyntaxTree &t, LuaDocParser &p);

    void FinishNode(LuaSyntaxTree &t, LuaDocParser &p);

    void BuildNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t);

    void BuildToken(LuaToken &token, LuaSyntaxTree &t);

    void BuildSyntax(LuaNodeOrToken n, LuaSyntaxTree& t);

    std::stack<std::size_t> _nodePosStack;
    std::size_t _tokenIndex;
};
