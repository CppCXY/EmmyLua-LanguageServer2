#include "LuaSyntaxTree.h"
#include <algorithm>
#include <fmt/format.h>
#include <ranges>


using enum LuaTokenKind;

LuaSyntaxTree::LuaSyntaxTree(std::shared_ptr<LuaFile> file)
    : _file(file) {
}

const LuaFile &LuaSyntaxTree::GetFile() const {
    return *_file;
}

std::size_t LuaSyntaxTree::GetStartOffset(std::size_t index) const {
    if (index == 0) {
        return 0;
    }
    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Node) {
            auto child = n.FirstChild;
            while (IsNode(child)) {
                child = GetFirstChild(child);
            }
            if (child != 0) {
                return _tokens[_nodeOrTokens[child].Data.TokenIndex].Start;
            } else {
                for (auto prevToken = index - 1; prevToken > 0; prevToken--) {
                    if (_nodeOrTokens[prevToken].Type == NodeOrTokenType::Token) {
                        auto &token = _tokens[_nodeOrTokens[prevToken].Data.TokenIndex];
                        return token.Start + token.Length;
                    }
                }
            }
        } else {
            return _tokens[n.Data.TokenIndex].Start;
        }
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetEndOffset(std::size_t index) const {
    if (index == 0) {
        return 0;
    }
    std::size_t nodeOrTokenIndex = index;
    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Node) {
            auto child = n.LastChild;
            while (IsNode(child)) {
                child = GetLastChild(child);
            }
            if (child != 0) {
                nodeOrTokenIndex = child;
            } else {
                for (auto prevToken = index - 1; prevToken > 0; prevToken--) {
                    if (_nodeOrTokens[prevToken].Type == NodeOrTokenType::Token) {
                        auto &token = _tokens[_nodeOrTokens[prevToken].Data.TokenIndex];
                        return token.Start + token.Length;
                    }
                }
                nodeOrTokenIndex = 0;
            }
        }
    } else if (!_nodeOrTokens.empty()) {
        nodeOrTokenIndex = _nodeOrTokens.size() - 1;
    } else {
        nodeOrTokenIndex = 0;
    };

    if (nodeOrTokenIndex != 0) {
        auto &token = _tokens[_nodeOrTokens[nodeOrTokenIndex].Data.TokenIndex];
        if (token.Length != 0) {
            return token.Start + token.Length - 1;
        } else {
            return token.Start;
        }
    }

    return 0;
}

std::size_t LuaSyntaxTree::GetNextSibling(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        return _nodeOrTokens[index].NextSibling;
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetPrevSibling(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        return _nodeOrTokens[index].PrevSibling;
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetFirstChild(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        return _nodeOrTokens[index].FirstChild;
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetLastChild(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        return _nodeOrTokens[index].LastChild;
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetFirstToken(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Node) {
            auto child = n.FirstChild;
            while (IsNode(child)) {
                child = GetFirstChild(child);
            }
            return child;
        } else {
            return index;
        }
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetLastToken(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Node) {
            auto child = n.LastChild;
            while (IsNode(child)) {
                child = GetLastChild(child);
            }
            return child;
        } else {
            return index;
        }
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetPrevToken(std::size_t index) const {
    if (index == 0) {
        return 0;
    }

    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Token) {
            auto tokenIndex = n.Data.TokenIndex;
            if (tokenIndex != 0) {
                return _tokens[tokenIndex - 1].NodeIndex;
            }
        } else {// Node, 可能存在无元素节点
            for (auto nodeIndex = index - 1; nodeIndex > 0; nodeIndex--) {
                if (IsToken(nodeIndex)) {
                    return nodeIndex;
                }
            }
        }
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetNextToken(std::size_t index) const {
    if (index == 0) {
        return 0;
    }

    std::size_t tokenNodeIndex = 0;
    if (index < _nodeOrTokens.size()) {
        auto &n = _nodeOrTokens[index];
        if (n.Type == NodeOrTokenType::Token) {
            tokenNodeIndex = index;
        } else {// Node, 可能存在无元素节点
            auto lastTokenIndex = GetLastToken(index);
            if (lastTokenIndex != 0) {
                tokenNodeIndex = lastTokenIndex;
            } else {
                // 当前节点是空节点, 则向前查找, 查找前一个token, 就可以轻松得到下一个token
                for (auto nodeIndex = index - 1; nodeIndex > 0; nodeIndex--) {
                    if (IsToken(nodeIndex)) {
                        tokenNodeIndex = nodeIndex;
                        break;
                    }
                }
            }
        }
    }

    if (tokenNodeIndex != 0) {
        auto &token = _nodeOrTokens[tokenNodeIndex];
        if (token.Data.TokenIndex + 1 < _tokens.size()) {
            return _tokens[token.Data.TokenIndex + 1].NodeIndex;
        }
    }
    return 0;
}

std::size_t LuaSyntaxTree::GetParent(std::size_t index) const {
    if (index < _nodeOrTokens.size()) {
        return _nodeOrTokens[index].Parent;
    }
    return 0;
}

LuaSyntaxNodeKind LuaSyntaxTree::GetNodeKind(std::size_t index) const {
    if (!IsNode(index)) {
        return LuaSyntaxNodeKind::None;
    }
    return _nodeOrTokens[index].Data.NodeKind;
}

LuaTokenKind LuaSyntaxTree::GetTokenKind(std::size_t index) const {
    if (!IsToken(index)) {
        return LuaTokenKind(0);
    }
    return _tokens[_nodeOrTokens[index].Data.TokenIndex].Kind;
}

bool LuaSyntaxTree::IsNode(std::size_t index) const {
    if (index == 0 || (_nodeOrTokens.size() <= index)) {
        return false;
    }
    return _nodeOrTokens[index].Type == NodeOrTokenType::Node;
}

bool LuaSyntaxTree::IsToken(std::size_t index) const {
    if (index == 0 || (_nodeOrTokens.size() <= index)) {
        return false;
    }
    return _nodeOrTokens[index].Type == NodeOrTokenType::Token;
}

const std::vector<LuaSyntaxNode> &LuaSyntaxTree::GetSyntaxNodes() const {
    return _syntaxNodes;
}

std::vector<LuaSyntaxNode> LuaSyntaxTree::GetTokens() const {
    std::vector<LuaSyntaxNode> results;
    if (_tokens.empty()) {
        return results;
    }

    results.reserve(_tokens.size());
    for (auto &token: _tokens) {
        results.emplace_back(token.NodeIndex);
    }

    return results;
}

LuaSyntaxNode LuaSyntaxTree::GetRootNode() const {
    return LuaSyntaxNode(1);
}

LuaSyntaxNode LuaSyntaxTree::GetTokenBeforeOffset(std::size_t offset) const {
    if (_tokens.empty()) {
        return LuaSyntaxNode();
    }

    auto tokenIt = std::partition_point(
            _tokens.begin(), _tokens.end(),
            [offset](const IncrementalToken &token) {
                return token.Start <= offset;
            });
    std::size_t tokenIndex = 0;
    if (tokenIt == _tokens.end()) {
        tokenIndex = _tokens.size() - 1;
    } else if (tokenIt == _tokens.begin()) {
        tokenIndex = 0;
    } else {
        tokenIndex = tokenIt - _tokens.begin() - 1;
    }

    auto &token = _tokens[tokenIndex];
    if (token.Start <= offset) {
        return LuaSyntaxNode(token.NodeIndex);
    }

    return LuaSyntaxNode();
}

LuaSyntaxNode LuaSyntaxTree::GetTokenAtOffset(std::size_t offset) const {
    if (_tokens.empty()) {
        return LuaSyntaxNode();
    }

    auto tokenIt = std::partition_point(
            _tokens.begin(), _tokens.end(),
            [offset](const IncrementalToken &token) {
                return token.Start <= offset;
            });
    std::size_t tokenIndex = 0;
    if (tokenIt == _tokens.end()) {
        tokenIndex = _tokens.size() - 1;
    } else if (tokenIt == _tokens.begin()) {
        tokenIndex = 0;
    } else {
        tokenIndex = tokenIt - _tokens.begin() - 1;
    }

    auto &token = _tokens[tokenIndex];
    if (token.Start <= offset && (token.Start + token.Length) > offset) {
        return LuaSyntaxNode(token.NodeIndex);
    }

    return LuaSyntaxNode();
}

std::string LuaSyntaxTree::GetDebugView() {
    std::string debugView;
    debugView.append("{ Lua Syntax Tree }\n");

    auto root = GetRootNode();
    std::stack<LuaSyntaxNode> traverseStack;
    std::size_t indent = 1;
    traverseStack.push(root);
    // 非递归深度优先遍历
    while (!traverseStack.empty()) {
        LuaSyntaxNode node = traverseStack.top();
        if (node.IsNode(*this)) {
            traverseStack.top() = LuaSyntaxNode(0);
            auto children = node.GetChildren(*this);
            for (auto& c: children | std::views::reverse) {
                traverseStack.push(c);
            }
            debugView.resize(debugView.size() + indent, '\t');
            debugView.append(fmt::format("{{ Node, index: {}, SyntaxKind: {} }}\n",
                                         node.GetIndex(),
                                         node.GetSyntaxKind(*this)));
            indent++;
        } else if (node.IsToken(*this)) {
            traverseStack.pop();
            debugView.resize(debugView.size() + indent, '\t');
            debugView.append(fmt::format("{{ Token, index: {}, TokenKind: {} }}\n",
                                         node.GetIndex(),
                                         node.GetText(*this)));
        } else {
            traverseStack.pop();
            indent--;
        }
    }
    return debugView;
}
