#pragma once

#include "LuaParser/Parser/LuaParser.h"
#include "LuaSyntaxTree.h"

class LuaTreeBuilder {
public:
    LuaTreeBuilder();

    void BuildTree(LuaSyntaxTree& t, LuaParser& p);

private:
    void StartNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaParser &p);

    void EatComments(LuaSyntaxTree &t, LuaParser &p);

    void EatInlineComment(LuaSyntaxTree &t, LuaParser &p);

    void EatToken(LuaSyntaxTree &t, LuaParser &p);

    bool IsEatAllComment( LuaSyntaxNodeKind kind,LuaSyntaxTree &t) const;

    void FinishNode(LuaSyntaxTree &t, LuaParser &p);

    void BuildNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t);

    void BuildToken(LuaToken &token, LuaSyntaxTree &t);

    std::stack<std::size_t> _nodePosStack;
    std::size_t _tokenIndex;
};
