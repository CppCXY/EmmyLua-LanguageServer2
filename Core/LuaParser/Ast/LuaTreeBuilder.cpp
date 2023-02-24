#include "LuaTreeBuilder.h"
#include "LuaParser/File/LuaFile.h"
#include <ranges>

using enum LuaTokenKind;

LuaTreeBuilder::LuaTreeBuilder()
    : _tokenIndex(0) {
}

void LuaTreeBuilder::BuildTree(LuaSyntaxTree &t, LuaParser &p) {

    StartNode(LuaSyntaxNodeKind::File, t, p);

    auto &events = p.GetParseState().GetEvents();
    std::vector<LuaSyntaxNodeKind> parents;
    for (auto &e: events) {
        switch (e.Type) {
            case MarkEventType::NodeStart: {
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

                for (const auto &parent: parents | std::views::reverse) {
                    StartNode(parent, t, p);
                }

                parents.clear();
                break;
            }
            case MarkEventType::EatToken: {
                EatComments(t, p);
                EatToken(t, p);
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
        auto commentCount = CalculateCommentCount(t, p);
        if (commentCount != 0) {
            auto edge = BindLeftComment(_tokenIndex + commentCount, t, p);
            assert(edge <= commentCount);
            EatCommentByCount(commentCount - edge, t, p);
            BuildNode(kind, t);
            EatCommentByCount(edge, t, p);
            return;
        }
    }
    BuildNode(kind, t);
}

std::size_t LuaTreeBuilder::CalculateCommentCount(LuaSyntaxTree &t, LuaParser &p) {
    auto &tokens = p.GetTokens();
    std::size_t count = 0;
    for (auto index = _tokenIndex; index < tokens.size(); index++) {
        switch (tokens[index].TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                count++;
                break;
            }
            default: {
                return count;
            }
        }
    }
    return count;
}

std::size_t LuaTreeBuilder::BindLeftComment(std::size_t startPos, LuaSyntaxTree &t, LuaParser &p) {
    auto &tokens = p.GetTokens();
    if (startPos >= tokens.size() || startPos <= _tokenIndex) {
        return 0;
    }
    auto &file = t.GetFile();
    auto &lineIndex = file.GetLineIndex();

    std::size_t line = lineIndex.GetLine(tokens[startPos].Range.StartOffset);
    std::size_t count = 0;
    std::size_t total = startPos - _tokenIndex;
    for (; count < total; count++) {
        auto index = startPos - count - 1;
        auto &token = tokens[index];
        switch (token.TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                auto commentLine = lineIndex.GetLine(token.Range.EndOffset);
                if (commentLine + 1 != line) {
                    return count;
                }
                line = lineIndex.GetLine(token.Range.StartOffset);
                break;
            }
            default: {
                return count;
            }
        }
    }
    return count;
}

std::size_t LuaTreeBuilder::BindRightComment(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaParser &p) {
    switch (kind) {
        case LuaSyntaxNodeKind::LocalStatement:
        case LuaSyntaxNodeKind::AssignStatement: {
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
                            return 1;
                        }
                        break;
                    }
                    default: {
                        return 0;
                    }
                }
            }
            break;
        }
        case LuaSyntaxNodeKind::Body:
        case LuaSyntaxNodeKind::TableFieldList:
        case LuaSyntaxNodeKind::File: {
            return CalculateCommentCount(t, p);
        }
        default: {
            return 0;
        }
    }
    return 0;
}

void LuaTreeBuilder::EatCommentByCount(std::size_t count, LuaSyntaxTree &t, LuaParser &p) {
    if (count == 0) {
        return;
    }

    auto &file = t.GetFile();
    auto &lineIndex = file.GetLineIndex();

    auto &tokens = p.GetTokens();
    std::vector<std::vector<std::size_t>> comments;
    comments.emplace_back();
    std::size_t line = 0;
    for (std::size_t i = 0; i < count; i++) {
        auto index = _tokenIndex + i;
        auto &token = tokens[index];
        auto commentLine = lineIndex.GetLine(token.Range.StartOffset);
        if (comments.back().empty() || (commentLine == line + 1)) {
            comments.back().emplace_back(index);
        } else {
            comments.emplace_back().emplace_back(index);
        }
        line = lineIndex.GetLine(token.Range.EndOffset);
    }

    for (auto &commentGroup: comments) {
        BuildComments(commentGroup, t, p);
    }
    _tokenIndex += count;
}

void LuaTreeBuilder::EatComments(LuaSyntaxTree &t, LuaParser &p) {
    auto count = CalculateCommentCount(t, p);
    EatCommentByCount(count, t, p);
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
        if (node.Type == NodeOrTokenType::Node) {
            auto edge = BindRightComment(node.Data.NodeKind, t, p);
            EatCommentByCount(edge, t, p);
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

void LuaTreeBuilder::BuildComments(std::vector<std::size_t> group, LuaSyntaxTree &t, LuaParser &p) {
    BuildNode(LuaSyntaxNodeKind::Comment, t);
    auto &tokens = p.GetTokens();
    for (auto i: group) {
        BuildToken(tokens[i], t);
    }

    FinishNode(t, p);
}
