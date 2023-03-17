#include "LuaDocTreeBuilder.h"
#include "LuaParser/SyntaxNode/Doc/DocAccessSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocAliasSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocClassSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocDeprecatedSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocEnumSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocFieldSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocGenericSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocInterfaceSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocOverloadSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocOverrideSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocParamSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocSeeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocSinceSyntax.h"
#include "LuaParser/SyntaxNode/Doc/DocTypeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/EnumFieldSyntax.h"
#include "LuaParser/SyntaxNode/Doc/FieldIndexSyntax.h"
#include "LuaParser/SyntaxNode/Doc/FuncTypeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/GenericDeclareSyntax.h"
#include "LuaParser/SyntaxNode/Doc/GenericDefListSyntax.h"
#include "LuaParser/SyntaxNode/Doc/TableFieldTypeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/TableTypeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/TypeSyntax.h"
#include "LuaParser/SyntaxNode/Doc/TypeSyntaxList.h"
#include <ranges>

using enum LuaTokenKind;

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
            auto docClassSyntax = t.CreateSyntax<DocClassSyntax>(n);
            docClassSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docClassSyntax->GenericDefList = t.GetMember<GenericDefListSyntax>(LuaSyntaxNodeKind::GenericDefList, n);
            docClassSyntax->ExtendTypeList = t.GetMember<TypeSyntaxList>(LuaSyntaxNodeKind::TypeList, n);
            break;
        }
        case LuaSyntaxNodeKind::DocInterface: {
            auto docInterfaceSyntax = t.CreateSyntax<DocInterfaceSyntax>(n);
            docInterfaceSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docInterfaceSyntax->GenericDefList = t.GetMember<GenericDefListSyntax>(LuaSyntaxNodeKind::GenericDefList, n);
            docInterfaceSyntax->ExtendTypeList = t.GetMember<TypeSyntaxList>(LuaSyntaxNodeKind::TypeList, n);
            break;
        }
        case LuaSyntaxNodeKind::DocAlias: {
            auto docAliasSyntax = t.CreateSyntax<DocAliasSyntax>(n);
            docAliasSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docAliasSyntax->Type = t.GetMember<TypeSyntax>(LuaSyntaxMultiKind::Type, n);
            break;
        }
        case LuaSyntaxNodeKind::DocEnum: {
            auto docEnumSyntax = t.CreateSyntax<DocEnumSyntax>(n);
            docEnumSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docEnumSyntax->SuperType = t.GetMember<TypeSyntax>(LuaSyntaxMultiKind::Type, n);
            docEnumSyntax->Fields = t.GetMembers<EnumFieldSyntax>(LuaSyntaxNodeKind::EnumField, n);
            break;
        }
        case LuaSyntaxNodeKind::DocType: {
            auto docTypeSyntax = t.CreateSyntax<DocTypeSyntax>(n);
            docTypeSyntax->TypeList = t.GetMember<TypeSyntaxList>(LuaSyntaxNodeKind::TypeList, n);
            break;
        }
        case LuaSyntaxNodeKind::DocField: {
            auto docFieldSyntax = t.CreateSyntax<DocFieldSyntax>(n);
            auto access = n.GetChildToken(TK_ACCESS, t);
            if (access.IsToken(t)) {
                auto accessText = access.GetText(t);
                if (accessText == "public") {
                    docFieldSyntax->Visibility = AccessVisibility::Public;
                } else if (accessText == "protected") {
                    docFieldSyntax->Visibility = AccessVisibility::Protected;
                } else if (accessText == "private") {
                    docFieldSyntax->Visibility = AccessVisibility::Private;
                }
            }

            docFieldSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docFieldSyntax->Index = t.GetMember<FieldIndexSyntax>(LuaSyntaxNodeKind::FieldIndex, n);
            docFieldSyntax->Nullable = n.GetChildToken(TK_NULLABLE, t).IsToken(t);
            docFieldSyntax->Type = t.GetMember<TypeSyntax>(LuaSyntaxMultiKind::Type, n);
            break;
        }
        case LuaSyntaxNodeKind::DocParam: {
            auto docParamSyntax = t.CreateSyntax<DocParamSyntax>(n);
            docParamSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            docParamSyntax->Nullable = n.GetChildToken(TK_NULLABLE, t).IsToken(t);
            docParamSyntax->Type = t.GetMember<TypeSyntax>(LuaSyntaxMultiKind::Type, n);
            break;
        }
        case LuaSyntaxNodeKind::DocGeneric: {
            auto docGenericSyntax = t.CreateSyntax<DocGenericSyntax>(n);
            docGenericSyntax->Declares = t.GetMembers<GenericDeclareSyntax>(LuaSyntaxNodeKind::GenericDeclare, n);
            break;
        }
        case LuaSyntaxNodeKind::DocOverload: {
            auto docOverloadSyntax = t.CreateSyntax<DocOverloadSyntax>(n);
            docOverloadSyntax->OverloadFunc = t.GetMember<FuncTypeSyntax>(LuaSyntaxNodeKind::FunctionType, n);
            break;
        }
        case LuaSyntaxNodeKind::DocSee: {
            auto docSeeSyntax = t.CreateSyntax<DocSeeSyntax>(n);
            docSeeSyntax->Content = n.GetChildToken(TK_NAME, t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::DocOverride: {
            t.CreateSyntax<DocOverrideSyntax>(n);
            break;
        }
        case LuaSyntaxNodeKind::DocPublic: {
            auto docAccessSyntax = t.CreateSyntax<DocAccessSyntax>(n);
            docAccessSyntax->Visibility = AccessVisibility::Public;
            break;
        }
        case LuaSyntaxNodeKind::DocPrivate: {
            auto docAccessSyntax = t.CreateSyntax<DocAccessSyntax>(n);
            docAccessSyntax->Visibility = AccessVisibility::Private;
            break;
        }
        case LuaSyntaxNodeKind::DocProtected: {
            auto docAccessSyntax = t.CreateSyntax<DocAccessSyntax>(n);
            docAccessSyntax->Visibility = AccessVisibility::Protected;
            break;
        }
        case LuaSyntaxNodeKind::DocDiagnostic:
            break;
        case LuaSyntaxNodeKind::DocSince: {
            auto docSinceSyntax = t.CreateSyntax<DocSinceSyntax>(n);
            docSinceSyntax->Content = n.GetChildToken(TK_NAME, t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::DocVersion:
            break;
        case LuaSyntaxNodeKind::DocDeprecated: {
            t.CreateSyntax<DocDeprecatedSyntax>(n);
            break;
        }
        case LuaSyntaxNodeKind::GenericDefList: {
            auto genericDefListSyntax = t.CreateSyntax<GenericDefListSyntax>(n);
            auto tokens = n.GetChildTokens(TK_NAME, t);
            for (auto token: tokens) {
                auto text = token.GetText(t);
                if (!text.empty()) {
                    genericDefListSyntax->TypeNameList.emplace_back(token.GetText(t));
                }
            }
            break;
        }
        case LuaSyntaxNodeKind::TypeList: {
            auto typeSyntaxList = t.CreateSyntax<TypeSyntaxList>(n);
            typeSyntaxList->TypeList = t.GetMembers<TypeSyntax>(LuaSyntaxMultiKind::Type, n);
            break;
        }
        case LuaSyntaxNodeKind::TableType: {
            auto tableTypeSyntax = t.CreateSyntax<TableTypeSyntax>(n);
            tableTypeSyntax->Fields = t.GetMembers<TableFieldTypeSyntax>(LuaSyntaxNodeKind::TableFieldType, n);
            break;
        }
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
