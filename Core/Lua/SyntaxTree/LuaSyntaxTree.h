#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "InnerNodeOrToken.h"
#include "LuaNodeOrToken.h"
#include "Lua/Define/LuaSyntaxError.h"
#include "Lua/Define/LuaToken.h"
#include "Lua/Source/LuaSource.h"
#include "Lua/SyntaxNode/BaseSyntax.h"
#include "TreeUpdateEvent.h"

class LuaSyntaxTree {
public:
    friend class LuaTreeBuilder;

    friend class LuaDocTreeBuilder;

    friend class LuaNodeOrToken;

    friend class TreeUpdateEvent;

    static std::unique_ptr<LuaSyntaxTree> ParseText(std::string &&text);

    explicit LuaSyntaxTree(LuaSource &&source);

    void Reset();

    const LuaSource &GetSource() const;

    LuaSource &GetSource();

    const std::vector<LuaNodeOrToken> &GetSyntaxNodes() const;

    std::vector<LuaNodeOrToken> GetTokens() const;

    LuaNodeOrToken GetRootNode() const;

    LuaNodeOrToken GetTokenBeforeOffset(std::size_t offset) const;

    LuaNodeOrToken GetTokenAtOffset(std::size_t offset) const;

    void ApplyUpdate(TreeUpdateEvent& treeUpdateEvent);

    template<class NodeSyntax>
    NodeSyntax *CreateSyntax(LuaNodeOrToken n) {
        auto pos = _syntaxNodes.size();
        auto &ptr = _syntaxNodes.emplace_back(std::make_unique<NodeSyntax>(n));
        _nodeOrTokens[n.GetIndex()].Data.Node.SyntaxIndex = pos;
        return dynamic_cast<NodeSyntax *>(ptr.get());
    }

    template<class NodeSyntax>
    NodeSyntax *GetSyntax(LuaNodeOrToken n) const {
        if (IsNode(n.GetIndex())) {
            auto &ptr = _syntaxNodes[_nodeOrTokens[n.GetIndex()].Data.Node.SyntaxIndex];
            return dynamic_cast<NodeSyntax *>(ptr.get());
        }
        return nullptr;
    }

    template<class SyntaxClass>
    SyntaxClass *GetMember(LuaSyntaxNodeKind kind, LuaNodeOrToken parent) const {
        auto n = parent.GetChildSyntaxNode(kind, *this);
        if (auto ptr = GetSyntax<SyntaxClass>(n); ptr) {
            return ptr;
        }
        return nullptr;
    }

    template<class SyntaxClass>
    SyntaxClass *GetMember(LuaSyntaxMultiKind kind, LuaNodeOrToken parent) const {
        auto n = parent.GetChildSyntaxNode(kind, *this);
        if (auto ptr = GetSyntax<SyntaxClass>(n); ptr) {
            return ptr;
        }
        return nullptr;
    }

    template<class SyntaxClass>
    std::vector<SyntaxClass *> GetMembers(LuaSyntaxMultiKind kind, LuaNodeOrToken parent) const {
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
    std::vector<SyntaxClass *> GetMembers(LuaSyntaxNodeKind kind, LuaNodeOrToken parent) const {
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

    std::string GetDebugSyntaxView();

    std::vector<LuaSyntaxError>& GetSyntaxErrors();
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
    std::vector<InnerNodeOrToken> _nodeOrTokens;
    std::vector<IncrementalToken> _tokens;
    std::vector<std::unique_ptr<BaseSyntax>> _syntaxNodes;
    std::vector<LuaSyntaxError> _errors;
};