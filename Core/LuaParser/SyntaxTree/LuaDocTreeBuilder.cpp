#include "LuaDocTreeBuilder.h"
#include <ranges>

LuaDocTreeBuilder::LuaDocTreeBuilder()
    : _tokenIndex(0) {
}

void LuaDocTreeBuilder::BuildTree(LuaSyntaxTree &t, LuaDocParser &p, std::size_t parent) {
    _nodePosStack.push(parent);

    StartNode(LuaSyntaxNodeKind::Comment, t, p);

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
}

void LuaDocTreeBuilder::StartNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaDocParser &p) {
    BuildNode(kind, t);
}

void LuaDocTreeBuilder::EatToken(LuaSyntaxTree &t, LuaDocParser &p) {
    auto &token = p.GetTokens()[_tokenIndex];
    _tokenIndex++;
    BuildToken(token, t);
}

void LuaDocTreeBuilder::FinishNode(LuaSyntaxTree &t, LuaDocParser &p) {
    if (!_nodePosStack.empty()) {
        auto nodePos = _nodePosStack.top();
        _nodePosStack.pop();
        BuildSyntax(LuaSyntaxNode(nodePos), t);
    }
}

void LuaDocTreeBuilder::BuildNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t) {
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

void LuaDocTreeBuilder::BuildToken(LuaToken &token, LuaSyntaxTree &t) {
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

void LuaDocTreeBuilder::BuildSyntax(LuaSyntaxNode n, LuaSyntaxTree &t) {
    switch (n.GetSyntaxKind(t)) {
        case LuaSyntaxNodeKind::DocClass: {
            auto docClassSyntax = t.CreateSyntax<>()
        }
        case LuaSyntaxNodeKind::DocInterface:
            break;
        case LuaSyntaxNodeKind::DocAlias:
            break;
        case LuaSyntaxNodeKind::DocEnum:
            break;
        case LuaSyntaxNodeKind::DocType:
            break;
        case LuaSyntaxNodeKind::DocField:
            break;
        case LuaSyntaxNodeKind::DocParam:
            break;
        case LuaSyntaxNodeKind::DocGeneric:
            break;
        case LuaSyntaxNodeKind::DocOverload:
            break;
        case LuaSyntaxNodeKind::DocSee:
            break;
        case LuaSyntaxNodeKind::DocOverride:
            break;
        case LuaSyntaxNodeKind::DocPublic:
            break;
        case LuaSyntaxNodeKind::DocPrivate:
            break;
        case LuaSyntaxNodeKind::DocProtected:
            break;
        case LuaSyntaxNodeKind::DocDiagnostic:
            break;
        case LuaSyntaxNodeKind::DocSince:
            break;
        case LuaSyntaxNodeKind::DocVersion:
            break;
        case LuaSyntaxNodeKind::DocDeprecated:
            break;
        case LuaSyntaxNodeKind::GenericDefList:
            break;
        case LuaSyntaxNodeKind::TypeList:
            break;
        case LuaSyntaxNodeKind::Type:
            break;
        case LuaSyntaxNodeKind::TableType:
            break;
        case LuaSyntaxNodeKind::TableFieldType:
            break;
        case LuaSyntaxNodeKind::ParType:
            break;
        case LuaSyntaxNodeKind::IdType:
            break;
        case LuaSyntaxNodeKind::StringType:
            break;
        case LuaSyntaxNodeKind::FunctionType:
            break;
        case LuaSyntaxNodeKind::FunctionTypeParamTypeList:
            break;
        case LuaSyntaxNodeKind::FunctionParam:
            break;
        case LuaSyntaxNodeKind::UnionType:
            break;
        case LuaSyntaxNodeKind::ArrayType:
            break;
        case LuaSyntaxNodeKind::GenericType:
            break;
        case LuaSyntaxNodeKind::GenericParamTypeList:
            break;
        case LuaSyntaxNodeKind::GenericDeclare:
            break;
        case LuaSyntaxNodeKind::FieldIndex:
            break;
        case LuaSyntaxNodeKind::EnumField:
            break;
        case LuaSyntaxNodeKind::NormalComment:
            break;
        default: {
            return;
        }
    }
}
