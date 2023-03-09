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

    friend class LuaSyntaxNode;

    static LuaSyntaxTree ParseText(std::string &&text);

    explicit LuaSyntaxTree(LuaSource &&source);

    void Reset();

    const LuaSource &GetSource() const;

    const std::vector<LuaSyntaxNode> &GetSyntaxNodes() const;

    std::vector<LuaSyntaxNode> GetTokens() const;

    LuaSyntaxNode GetRootNode() const;

    LuaSyntaxNode GetTokenBeforeOffset(std::size_t offset) const;

    LuaSyntaxNode GetTokenAtOffset(std::size_t offset) const;

    template<class NodeSyntax>
    NodeSyntax *CreateSyntax(LuaSyntaxNode n) {
        auto pos = _syntaxNodes.size();
        auto &ptr = _syntaxNodes.emplace_back(std::make_unique<NodeSyntax>(n));
        _nodeOrTokens[n.GetIndex()].Data.Node.SyntaxIndex = pos;
        return dynamic_cast<NodeSyntax *>(ptr.get());
    }

    template<class NodeSyntax>
    NodeSyntax *GetSyntax(LuaSyntaxNode n) const {
        if (IsNode(n.GetIndex())) {
            auto &ptr = _syntaxNodes[_nodeOrTokens[n.GetIndex()].Data.Node.SyntaxIndex];
            return dynamic_cast<NodeSyntax *>(ptr.get());
        }
        return nullptr;
    }

    template<class SyntaxClass>
    SyntaxClass *GetMember(LuaSyntaxNodeKind kind, LuaSyntaxNode parent) const {
        auto n = parent.GetChildSyntaxNode(kind, *this);
        if (auto ptr = GetSyntax<SyntaxClass>(n); ptr) {
            return ptr;
        }
        return nullptr;
    }

    template<class SyntaxClass>
    SyntaxClass *GetMember(LuaSyntaxMultiKind kind, LuaSyntaxNode parent) const {
        auto n = parent.GetChildSyntaxNode(kind, *this);
        if (auto ptr = GetSyntax<SyntaxClass>(n); ptr) {
            return ptr;
        }
        return nullptr;
    }

    template<class SyntaxClass>
    std::vector<SyntaxClass *> GetMembers(LuaSyntaxMultiKind kind, LuaSyntaxNode parent) const {
        auto children = parent.GetChildSyntaxNodes(kind, *this);
        std::vector<SyntaxClass *> results;
        for (auto child: children) {
            if (auto ptr = GetSyntax<SyntaxClass>(child); ptr) {
                results.push_back(ptr);
            }
        }
        return results;
    }

    template<class SyntaxClass>
    std::vector<SyntaxClass *> GetMembers(LuaSyntaxNodeKind kind, LuaSyntaxNode parent) const {
        auto children = parent.GetChildSyntaxNodes(kind, *this);
        std::vector<SyntaxClass *> results;
        for (auto child: children) {
            if (auto ptr = GetSyntax<SyntaxClass>(child); ptr) {
                results.push_back(ptr);
            }
        }
        return results;
    }

    std::string GetDebugView();

private:
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

    TextRange GetTokenRange(std::size_t index) const;

    bool IsNode(std::size_t index) const;

    bool IsToken(std::size_t index) const;

    LuaSource _source;
    std::vector<NodeOrToken> _nodeOrTokens;
    std::vector<IncrementalToken> _tokens;
    std::vector<std::unique_ptr<BaseSyntax>> _syntaxNodes;
};