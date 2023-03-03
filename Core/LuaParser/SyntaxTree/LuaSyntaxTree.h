#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "LuaParser/Define/LuaSyntaxError.h"
#include "LuaParser/Define/LuaToken.h"
#include "LuaParser/Source/LuaSource.h"
#include "LuaParser/SyntaxNode/BaseSyntax.h"
#include "LuaParser/SyntaxNode/LuaSyntaxNode.h"
#include "NodeOrToken.h"

class LuaSyntaxTree {
public:
    friend class LuaTreeBuilder;

    friend class LuaDocTreeBuilder;

    static LuaSyntaxTree ParseText(std::string&& text);

    explicit LuaSyntaxTree(LuaSource&& source);

    void Reset();

    const LuaSource &GetSource() const;

    std::size_t GetStartOffset(std::size_t index) const;

    std::size_t GetEndOffset(std::size_t index) const;

    std::size_t GetNextSibling(std::size_t index) const;

    std::size_t GetPrevSibling(std::size_t index) const;

    std::size_t GetFirstChild(std::size_t index) const;

    std::size_t GetLastChild(std::size_t index) const;

    std::size_t GetFirstToken(std::size_t index) const;

    std::size_t GetLastToken(std::size_t index) const;

    std::size_t GetPrevToken(std::size_t index) const;

    std::size_t GetNextToken(std::size_t index) const;

    std::size_t GetParent(std::size_t index) const;

    LuaSyntaxNodeKind GetNodeKind(std::size_t index) const;

    LuaTokenKind GetTokenKind(std::size_t index) const;

    bool IsNode(std::size_t index) const;

    bool IsToken(std::size_t index) const;

    const std::vector<LuaSyntaxNode>& GetSyntaxNodes() const;

    std::vector<LuaSyntaxNode> GetTokens() const;

    LuaSyntaxNode GetRootNode() const;

    LuaSyntaxNode GetTokenBeforeOffset(std::size_t offset) const;

    LuaSyntaxNode GetTokenAtOffset(std::size_t offset) const;

    std::string GetDebugView();
private:
    LuaSource _source;
    std::vector<NodeOrToken> _nodeOrTokens;
    std::vector<IncrementalToken> _tokens;
    std::vector<std::unique_ptr<BaseSyntax>> _syntaxNodes;
};