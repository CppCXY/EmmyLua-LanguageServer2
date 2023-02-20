#include "LuaTreeBuilder.h"

using enum LuaTokenKind;

LuaTreeBuilder::LuaTreeBuilder()
    : _tokenIndex(0) {
}

void LuaTreeBuilder::BuildTree(LuaSyntaxTree &t, LuaParser &p) {
    StartNode(LuaSyntaxNodeKind::File, t, p);

    auto &events = p.GetParseState().GetEvents();
    std::vector<LuaSyntaxNodeKind> parents;
    for (std::size_t i = 0; i != events.size(); i++) {
        switch (events[i].Type) {
            case MarkEventType::NodeStart: {
                auto e = events[i];
                if (e.U.Start.Kind == LuaSyntaxNodeKind::None) {
                    continue;
                }

                parents.push_back(e.U.Start.Kind);
                auto parentPos = e.U.Start.Parent;
                while (parentPos > 0) {
                    if (events[parentPos].Type == MarkEventType::NodeStart) {
                        auto &pe = events[parentPos];
                        parents.push_back(pe.U.Start.Kind);
                        parentPos = pe.U.Start.Parent;
                        pe.U.Start.Kind = LuaSyntaxNodeKind::None;
                    } else {
                        break;
                    }
                }
                // When debug here will throw error
                for (auto rIt = parents.rbegin(); rIt != parents.rend(); rIt++) {
                    StartNode(*rIt, t, p);
                }

                parents.clear();
                break;
            }
            case MarkEventType::EatToken: {
                EatComments(t, p);
                EatToken(t, p);
                EatInlineComment(t, p);
                break;
            }
            case MarkEventType::NodeEnd: {
                FinishNode(t, p);
                break;
            }
            default: {
                break;
            }
        }
    }

    FinishNode(t, p);

    if (!t._nodeOrTokens.empty()) {
        t._syntaxNodes.reserve(t._nodeOrTokens.size() - 1);
        for (std::size_t i = 0; i != t._nodeOrTokens.size() - 1; i++) {
            t._syntaxNodes.emplace_back(i + 1);
        }
    }
}

void LuaTreeBuilder::StartNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaParser &p) {
    if (!IsEatAllComment(kind, t)) {
        EatComments(t, p);
        BuildNode(kind, t);
    } else {
        BuildNode(kind, t);
    }
}

void LuaTreeBuilder::EatComments(LuaSyntaxTree &t, LuaParser &p) {
    auto &tokens = p.GetTokens();
    for (auto index = _tokenIndex; index < tokens.size(); index++) {
        switch (tokens[index].TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                EatToken(t, p);
                break;
            }
            default: {
                return;
            }
        }
    }
}

void LuaTreeBuilder::EatInlineComment(LuaSyntaxTree &t, LuaParser &p) {
    auto &tokens = p.GetTokens();
    if (_tokenIndex > 0 && _tokenIndex < tokens.size()) {
        auto index = _tokenIndex;
        switch (tokens[index].TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                auto prevToken = tokens[index - 1];
                auto &file = t.GetFile();
                if (file.GetLineIndex().GetLine(prevToken.Range.EndOffset) == file.GetLineIndex().GetLine(tokens[index].Range.StartOffset)) {
                    EatToken(t, p);
                }
                break;
            }
            default: {
                return;
            }
        }
    }
}

void LuaTreeBuilder::EatToken(LuaSyntaxTree &t, LuaParser &p) {
    auto &token = p.GetTokens()[_tokenIndex];
    _tokenIndex++;
    BuildToken(token, t);
}

void LuaTreeBuilder::FinishNode(LuaSyntaxTree &t, LuaParser &p) {
    if (!_nodePosStack.empty()) {
        auto nodePos = _nodePosStack.top();
        auto &node = t._nodeOrTokens[nodePos];
        if (node.Type == NodeOrTokenType::Node &&
            IsEatAllComment(node.Data.NodeKind, t)) {
            EatComments(t, p);
        } else {
            if (_tokenIndex < p.GetTokens().size() && _tokenIndex > 0) {
                EatInlineComment(t, p);
            }
        }

        _nodePosStack.pop();
    }
}

void LuaTreeBuilder::BuildNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t) {
    auto currentPos = t._nodeOrTokens.size();
    auto &currentNode = t._nodeOrTokens.emplace_back(kind);
    if (!_nodePosStack.empty()) {
        auto parentIndex = _nodePosStack.top();
        auto &parentNode = t._nodeOrTokens[parentIndex];
        if (parentNode.FirstChild == 0) {
            parentNode.FirstChild = currentPos;
            parentNode.LastChild = currentPos;
        } else {
            auto &lastNode = t._nodeOrTokens[parentNode.LastChild];
            lastNode.NextSibling = currentPos;
            currentNode.PrevSibling = parentNode.LastChild;
            parentNode.LastChild = currentPos;
        }
        currentNode.Parent = parentIndex;
    }

    _nodePosStack.push(currentPos);
}

void LuaTreeBuilder::BuildToken(LuaToken &token, LuaSyntaxTree &t) {
    auto currentNodePos = t._nodeOrTokens.size();
    auto currentTokenPos = t._tokens.size();

    t._tokens.emplace_back(token, currentNodePos);
    auto &currentToken = t._nodeOrTokens.emplace_back(currentTokenPos);
    if (!_nodePosStack.empty()) {
        auto parentIndex = _nodePosStack.top();
        auto &parentNode = t._nodeOrTokens[parentIndex];
        if (parentNode.FirstChild == 0) {
            parentNode.FirstChild = currentNodePos;
            parentNode.LastChild = currentNodePos;
        } else {
            auto &lastNode = t._nodeOrTokens[parentNode.LastChild];
            lastNode.NextSibling = currentNodePos;
            currentToken.PrevSibling = parentNode.LastChild;
            parentNode.LastChild = currentNodePos;
        }
        currentToken.Parent = parentIndex;
    }
}

bool LuaTreeBuilder::IsEatAllComment(LuaSyntaxNodeKind kind, LuaSyntaxTree &t) const {
    return kind == LuaSyntaxNodeKind::Body || kind == LuaSyntaxNodeKind::TableFieldList || kind == LuaSyntaxNodeKind::File;
}
