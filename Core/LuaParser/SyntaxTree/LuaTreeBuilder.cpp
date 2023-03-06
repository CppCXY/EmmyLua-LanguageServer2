#include "LuaTreeBuilder.h"
#include "LuaDocTreeBuilder.h"
#include "LuaParser/DocLexer/LuaDocLexer.h"
#include "LuaParser/SyntaxNode/Doc/CommentSyntax.h"
#include "LuaParser/SyntaxNode/Lua/BodySyntax.h"
#include "LuaParser/SyntaxNode/Lua/BreakStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/DoStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/ExprSyntax.h"
#include "LuaParser/SyntaxNode/Lua/ExprSyntaxList.h"
#include "LuaParser/SyntaxNode/Lua/ForStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/FunctionNameExprSyntax.h"
#include "LuaParser/SyntaxNode/Lua/FunctionStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/IfStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/LabelStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/LocalFunctionStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/LocalStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/NameDefSyntax.h"
#include "LuaParser/SyntaxNode/Lua/NameDefSyntaxList.h"
#include "LuaParser/SyntaxNode/Lua/ParamSyntaxList.h"
#include "LuaParser/SyntaxNode/Lua/RepeatStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/ReturnStatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/StatementSyntax.h"
#include "LuaParser/SyntaxNode/Lua/WhileStatementSyntax.h"
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
                EatTrivias(t, p);
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

    //    if (!t._nodeOrTokens.empty()) {
    //        t._syntaxNodes.reserve(t._nodeOrTokens.size() - 1);
    //        for (std::size_t i = 0; i != t._nodeOrTokens.size() - 1; i++) {
    //            t._syntaxNodes.emplace_back(i + 1);
    //        }
    //    }
}

void LuaTreeBuilder::StartNode(LuaSyntaxNodeKind kind, LuaSyntaxTree &t, LuaParser &p) {
    if (!IsEatAllTrivia(kind, t)) {
        auto triviaCount = CalculateTriviaCount(t, p);
        if (triviaCount != 0) {
            auto edge = BindLeftComment(_tokenIndex + triviaCount, t, p);
            assert(edge <= triviaCount);
            EatTriviaByCount(triviaCount - edge, t, p);
            BuildNode(kind, t);
            EatTriviaByCount(edge, t, p);
            return;
        }
    }
    BuildNode(kind, t);
}

std::size_t LuaTreeBuilder::CalculateTriviaCount(LuaSyntaxTree &t, LuaParser &p) {
    auto &tokens = p.GetTokens();
    std::size_t count = 0;
    for (auto index = _tokenIndex; index < tokens.size(); index++) {
        switch (tokens[index].TokenType) {
            case TK_WS:
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
    auto &file = t.GetSource();
    auto &lineIndex = file.GetLineIndex();

    std::size_t count = 0;
    std::size_t total = startPos - _tokenIndex;
    for (; count < total; count++) {
        auto index = startPos - count - 1;
        auto &token = tokens[index];
        switch (token.TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                break;
            }
            case TK_WS: {
                if (lineIndex.GetLine(token.Range.StartOffset) + 1 < lineIndex.GetLine(token.Range.EndOffset + 1)) {
                    return count;
                }
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
                std::size_t wsCount = 0;
                switch (tokens[index].TokenType) {
                    case TK_SHORT_COMMENT:
                    case TK_LONG_COMMENT:
                    case TK_SHEBANG: {
                        auto prevToken = tokens[index - 1];
                        auto &file = t.GetSource();
                        if (file.GetLineIndex().GetLine(prevToken.Range.EndOffset) == file.GetLineIndex().GetLine(tokens[index].Range.StartOffset)) {
                            return 1 + wsCount;
                        }
                        break;
                    }
                    case TK_WS: {
                        wsCount++;
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
            return CalculateTriviaCount(t, p);
        }
        default: {
            return 0;
        }
    }
    return 0;
}

void LuaTreeBuilder::EatTriviaByCount(std::size_t count, LuaSyntaxTree &t, LuaParser &p) {
    if (count == 0) {
        return;
    }
    enum class ParseState {
        Init,
        Comment
    } state = ParseState::Init;


    auto &source = t.GetSource();
    auto &lineIndex = source.GetLineIndex();

    auto &tokens = p.GetTokens();

    std::vector<std::size_t> comments;
    for (std::size_t i = 0; i < count; i++) {
        auto index = _tokenIndex + i;
        auto &token = tokens[index];
        switch (state) {
            case ParseState::Init: {
                if (token.TokenType == TK_WS) {
                    BuildToken(token, t);
                } else {
                    comments.push_back(index);
                    state = ParseState::Comment;
                }
                break;
            }
            case ParseState::Comment: {
                if (index == count) {
                    if (token.TokenType == TK_WS) {
                        BuildComments(comments, t, p);
                        BuildToken(token, t);
                    } else {
                        comments.push_back(index);
                        BuildComments(comments, t, p);
                    }
                    break;
                }

                if (token.TokenType == TK_WS) {
                    if (lineIndex.GetLine(token.Range.StartOffset) + 1 >= lineIndex.GetLine(token.Range.EndOffset + 1)) {
                        comments.push_back(index);
                    } else {
                        BuildComments(comments, t, p);
                        comments.clear();
                        BuildToken(token, t);
                        state = ParseState::Init;
                    }
                } else {
                    comments.push_back(index);
                }
                break;
            }
        }
    }

    _tokenIndex += count;
}

void LuaTreeBuilder::EatTrivias(LuaSyntaxTree &t, LuaParser &p) {
    auto count = CalculateTriviaCount(t, p);
    EatTriviaByCount(count, t, p);
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
            auto edge = BindRightComment(node.Data.Node.Kind, t, p);
            EatTriviaByCount(edge, t, p);

            BuildSyntax(LuaSyntaxNode(nodePos), t);
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

bool LuaTreeBuilder::IsEatAllTrivia(LuaSyntaxNodeKind kind, LuaSyntaxTree &t) const {
    return kind == LuaSyntaxNodeKind::Body || kind == LuaSyntaxNodeKind::TableFieldList || kind == LuaSyntaxNodeKind::File;
}

void LuaTreeBuilder::BuildComments(std::vector<std::size_t> &group, LuaSyntaxTree &t, LuaParser &p) {
    if (_nodePosStack.empty()) {
        return;
    }

    const auto &file = t.GetSource();
    auto &tokens = p.GetTokens();
    std::vector<LuaToken> luaDocTokens;
    for (auto i: group) {
        auto &luaToken = tokens[i];
        LuaDocLexer docLexer(file.Slice(luaToken.Range.StartOffset, luaToken.Range.EndOffset), luaToken.Range.StartOffset);
        for (auto &docToken: docLexer.Tokenize()) {
            luaDocTokens.emplace_back(docToken);
        }
    }

    LuaDocParser docParser(&file, std::move(luaDocTokens));
    docParser.Parse();
    LuaDocTreeBuilder docTreeBuilder;

    docTreeBuilder.BuildTree(t, docParser, _nodePosStack.top());
}

void LuaTreeBuilder::BuildSyntax(LuaSyntaxNode n, LuaSyntaxTree &t) {
    switch (n.GetSyntaxKind(t)) {
        case LuaSyntaxNodeKind::Body: {
            auto bodySyntax = t.CreateSyntax<BodySyntax>(n);
            bodySyntax->Statements = t.GetMembers<StatementSyntax>(LuaSyntaxMultiKind::Statement, n);
            break;
        }
        case LuaSyntaxNodeKind::EmptyStatement: {
            break;
        }
        case LuaSyntaxNodeKind::LocalStatement: {
            auto localSyntax = t.CreateSyntax<LocalStatementSyntax>(n);
            localSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            localSyntax->NameDefList = t.GetMember<NameDefSyntaxList>(LuaSyntaxNodeKind::NameDefList, n);
            localSyntax->ExprList = t.GetMember<ExprSyntaxList>(LuaSyntaxNodeKind::ExpressionList, n);
            break;
        }
        case LuaSyntaxNodeKind::LocalFunctionStatement: {
            auto localFuncSyntax = t.CreateSyntax<LocalFunctionStatementSyntax>(n);
            localFuncSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            localFuncSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            auto funcBody = n.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
            if (funcBody.IsNode(t)) {
                localFuncSyntax->ParamSyntaxList = t.GetMember<ParamSyntaxList>(LuaSyntaxNodeKind::ParamList, funcBody);
                localFuncSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, funcBody);
            }
            break;
        }
        case LuaSyntaxNodeKind::IfStatement: {
            auto ifSyntax = t.CreateSyntax<IfStatementSyntax>(n);
            ifSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            ifSyntax->IfExprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            ifSyntax->Bodys = t.GetMembers<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            if (ifSyntax->IfExprs.size() < ifSyntax->Bodys.size()) {
                ifSyntax->ElseBody = ifSyntax->Bodys.back();
            }

            break;
        }
        case LuaSyntaxNodeKind::WhileStatement: {
            auto whileSyntax = t.CreateSyntax<WhileStatementSyntax>(n);
            whileSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            whileSyntax->ConditionExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            whileSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            break;
        }
        case LuaSyntaxNodeKind::DoStatement: {
            auto doSyntax = t.CreateSyntax<DoStatementSyntax>(n);
            doSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            doSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            break;
        }
        case LuaSyntaxNodeKind::ForStatement: {
            auto forSyntax = t.CreateSyntax<ForStatementSyntax>(n);
            forSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            auto fori = n.GetChildSyntaxNode(LuaSyntaxNodeKind::ForNumber, t);
            if (fori.IsNode(t)) {
                forSyntax->IsForNumber = true;
                forSyntax->Exprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, fori);
                forSyntax->Names.emplace_back(fori.GetChildToken(TK_NAME, t).GetText(t));
                break;
            }
            auto forList = n.GetChildSyntaxNode(LuaSyntaxNodeKind::ForList, t);
            if (fori.IsNode(t)) {
                forSyntax->IsForList = true;
                auto nameDefList = forList.GetChildSyntaxNode(LuaSyntaxNodeKind::NameDefList, t);
                for (auto nameDef: nameDefList.GetChildSyntaxNodes(LuaSyntaxNodeKind::NameDef, t)) {
                    forSyntax->Names.emplace_back(nameDef.GetChildToken(TK_NAME, t).GetText(t));
                }

                auto exprList = forList.GetChildSyntaxNode(LuaSyntaxNodeKind::ExpressionList, t);
                for (auto expr: exprList.GetChildSyntaxNodes(LuaSyntaxMultiKind::Expression, t)) {
                    forSyntax->Exprs.emplace_back(t.GetSyntax<ExprSyntax>(expr));
                }
            }

            break;
        }
        case LuaSyntaxNodeKind::RepeatStatement: {
            auto repeatSyntax = t.CreateSyntax<RepeatStatementSyntax>(n);
            repeatSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            repeatSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            repeatSyntax->ConditionExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::FunctionStatement: {
            auto funcSyntax = t.CreateSyntax<FunctionStatementSyntax>(n);
            funcSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            funcSyntax->FunctionName = t.GetMember<FunctionNameExprSyntax>(LuaSyntaxNodeKind::FunctionNameExpression, n);
            auto funcBody = n.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
            if (funcBody.IsNode(t)) {
                funcSyntax->ParamSyntaxList = t.GetMember<ParamSyntaxList>(LuaSyntaxNodeKind::ParamList, funcBody);
                funcSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, funcBody);
            }
            break;
        }
        case LuaSyntaxNodeKind::LabelStatement: {
            auto labelSyntax = t.CreateSyntax<LabelStatementSyntax>(n);
            labelSyntax->LabelName = n.GetChildToken(TK_NAME, t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::BreakStatement: {
            auto breakSyntax = t.CreateSyntax<BreakStatementSyntax>(n);
            auto p = n.Ancestor(t, [](LuaSyntaxNodeKind kind, bool &ct) -> bool {
                switch (kind) {
                    case LuaSyntaxNodeKind::ForStatement:
                    case LuaSyntaxNodeKind::WhileStatement:
                    case LuaSyntaxNodeKind::RepeatStatement: {
                        return true;
                    }
                    case LuaSyntaxNodeKind::FunctionBody: {
                        ct = false;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                return false;
            });
            breakSyntax->LoopStatement = t.GetSyntax<StatementSyntax>(p);
            break;
        }
        case LuaSyntaxNodeKind::ReturnStatement: {
            auto returnSyntax = t.CreateSyntax<ReturnStatementSyntax>(n);
            returnSyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            returnSyntax->ReturnExprList = t.GetMember<ExprSyntaxList>(LuaSyntaxNodeKind::ExpressionList, n);
            break;
        }
        case LuaSyntaxNodeKind::GotoStatement:
        {
            auto gotoSyntax = t.CreateSyntax<GotoStatementSyntax>(n);
//            gotoSyntax->LabelName =

            break;
        }
        case LuaSyntaxNodeKind::ExpressionStatement:
            break;
        case LuaSyntaxNodeKind::AssignStatement:
            break;
        case LuaSyntaxNodeKind::SuffixedExpression:
            break;
        case LuaSyntaxNodeKind::ParExpression:
            break;
        case LuaSyntaxNodeKind::LiteralExpression:
            break;
        case LuaSyntaxNodeKind::StringLiteralExpression:
            break;
        case LuaSyntaxNodeKind::ClosureExpression:
            break;
        case LuaSyntaxNodeKind::UnaryExpression:
            break;
        case LuaSyntaxNodeKind::BinaryExpression:
            break;
        case LuaSyntaxNodeKind::TableExpression:
            break;
        case LuaSyntaxNodeKind::CallExpression:
            break;
        case LuaSyntaxNodeKind::IndexExpression:
            break;
        case LuaSyntaxNodeKind::NameExpression:
            break;
        case LuaSyntaxNodeKind::FunctionNameExpression:
            break;
        case LuaSyntaxNodeKind::VarList:
            break;
        case LuaSyntaxNodeKind::TableFieldList:
            break;
        case LuaSyntaxNodeKind::TableField:
            break;
        case LuaSyntaxNodeKind::TableFieldSep:
            break;
        case LuaSyntaxNodeKind::FunctionBody:
            break;
        case LuaSyntaxNodeKind::ParamList:
            break;
        case LuaSyntaxNodeKind::NameDefList:
            break;
        case LuaSyntaxNodeKind::Attribute:
            break;
        case LuaSyntaxNodeKind::ExpressionList:
            break;
        case LuaSyntaxNodeKind::ForNumber:
            break;
        case LuaSyntaxNodeKind::ForList:
            break;
        case LuaSyntaxNodeKind::Error:
            break;
        default: {
        }
    }
}
