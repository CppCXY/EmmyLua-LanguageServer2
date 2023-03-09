#include "LuaTreeBuilder.h"
#include "LuaDocTreeBuilder.h"
#include "LuaParser/DocLexer/LuaDocLexer.h"
#include "LuaParser/SyntaxNode/Lua/LuaSyntaxs.h"
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
                if (lineIndex.GetLine(token.Range.StartOffset) + 1 < lineIndex.GetLine(token.Range.GetEndOffset() + 1)) {
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
                        if (file.GetLineIndex().GetLine(prevToken.Range.GetEndOffset()) == file.GetLineIndex().GetLine(tokens[index].Range.StartOffset)) {
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
                    if (lineIndex.GetLine(token.Range.StartOffset) + 1 >= lineIndex.GetLine(token.Range.GetEndOffset() + 1)) {
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
        LuaDocLexer docLexer(file.Slice(luaToken.Range), luaToken.Range.StartOffset);
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
            bodySyntax->Comments = t.GetMembers<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            bodySyntax->Stmts = t.GetMembers<StmtSyntax>(LuaSyntaxMultiKind::Statement, n);
            break;
        }
        case LuaSyntaxNodeKind::LocalStatement: {
            auto localSyntax = t.CreateSyntax<LocalStmtSyntax>(n);
            localSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            localSyntax->NameDefList = t.GetMember<NameDefSyntaxList>(LuaSyntaxNodeKind::NameDefList, n);
            localSyntax->ExprList = t.GetMember<ExprSyntaxList>(LuaSyntaxNodeKind::ExpressionList, n);
            break;
        }
        case LuaSyntaxNodeKind::LocalFunctionStatement: {
            auto localFuncSyntax = t.CreateSyntax<LocalFuncStmtSyntax>(n);
            localFuncSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            localFuncSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            auto funcBody = n.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
            if (funcBody.IsNode(t)) {
                localFuncSyntax->ParamList = t.GetMember<ParamSyntaxList>(LuaSyntaxNodeKind::ParamList, funcBody);
                localFuncSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, funcBody);
            }
            break;
        }
        case LuaSyntaxNodeKind::IfStatement: {
            auto ifSyntax = t.CreateSyntax<IfStmtSyntax>(n);
            ifSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            ifSyntax->IfExprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            ifSyntax->Bodys = t.GetMembers<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            if (ifSyntax->IfExprs.size() < ifSyntax->Bodys.size()) {
                ifSyntax->ElseBody = ifSyntax->Bodys.back();
            }

            break;
        }
        case LuaSyntaxNodeKind::WhileStatement: {
            auto whileSyntax = t.CreateSyntax<WhileStmtSyntax>(n);
            whileSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            whileSyntax->ConditionExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            whileSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            break;
        }
        case LuaSyntaxNodeKind::DoStatement: {
            auto doSyntax = t.CreateSyntax<DoStmtSyntax>(n);
            doSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            doSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            break;
        }
        case LuaSyntaxNodeKind::ForStatement: {
            auto forSyntax = t.CreateSyntax<ForStmtSyntax>(n);
            forSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
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
            auto repeatSyntax = t.CreateSyntax<RepeatStmtSyntax>(n);
            repeatSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            repeatSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, n);
            repeatSyntax->ConditionExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::FunctionStatement: {
            auto funcSyntax = t.CreateSyntax<FuncStmtSyntax>(n);
            funcSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            funcSyntax->FuncName = t.GetMember<FuncNameExprSyntax>(LuaSyntaxNodeKind::FunctionNameExpression, n);
            auto funcBody = n.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
            if (funcBody.IsNode(t)) {
                funcSyntax->ParamList = t.GetMember<ParamSyntaxList>(LuaSyntaxNodeKind::ParamList, funcBody);
                funcSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, funcBody);
            }
            break;
        }
        case LuaSyntaxNodeKind::LabelStatement: {
            auto labelSyntax = t.CreateSyntax<LabelStmtSyntax>(n);
            labelSyntax->LabelName = n.GetChildToken(TK_NAME, t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::BreakStatement: {
            auto breakSyntax = t.CreateSyntax<BreakStmtSyntax>(n);
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
            breakSyntax->LoopStatement = t.GetSyntax<StmtSyntax>(p);
            break;
        }
        case LuaSyntaxNodeKind::ReturnStatement: {
            auto returnSyntax = t.CreateSyntax<ReturnStmtSyntax>(n);
            returnSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            returnSyntax->ReturnExprList = t.GetMember<ExprSyntaxList>(LuaSyntaxNodeKind::ExpressionList, n);
            break;
        }
        case LuaSyntaxNodeKind::GotoStatement: {
            auto gotoSyntax = t.CreateSyntax<GotoStmtSyntax>(n);
            gotoSyntax->Label = n.GetChildToken(TK_NAME, t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::CallStatement: {
            auto exprStmtSyntax = t.CreateSyntax<CallStmtSyntax>(n);
            exprStmtSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            exprStmtSyntax->Expr = t.GetMember<SuffixedExprSyntax>(LuaSyntaxNodeKind::SuffixedExpression, n);
            break;
        }
        case LuaSyntaxNodeKind::AssignStatement: {
            auto assignStmtSyntax = t.CreateSyntax<AssignStmtSyntax>(n);
            assignStmtSyntax->Comment = t.GetMember<CommentSyntax>(LuaSyntaxNodeKind::Comment, n);
            assignStmtSyntax->VarList = t.GetMember<VarSyntaxList>(LuaSyntaxNodeKind::VarList, n);
            assignStmtSyntax->ExprList = t.GetMember<ExprSyntaxList>(LuaSyntaxNodeKind::ExpressionList, n);
            break;
        }
        case LuaSyntaxNodeKind::SuffixedExpression: {
            auto suffixedExprSyntax = t.CreateSyntax<SuffixedExprSyntax>(n);
            suffixedExprSyntax->Exprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::ParExpression: {
            auto parExprSyntax = t.CreateSyntax<ParExprSyntax>(n);
            parExprSyntax->InnerExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::LiteralExpression: {
            auto literalExprSyntax = t.CreateSyntax<LiteralExprSyntax>(n);
            auto firstToken = n.GetFirstToken(t);
            literalExprSyntax->Literal = firstToken.GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::StringLiteralExpression: {
            auto stringLiteralExprSyntax = t.CreateSyntax<StringLiteralExprSyntax>(n);
            auto text = n.GetFirstToken(t).GetText(t);
            if (!text.empty()) {
                // TODO complete implement
                stringLiteralExprSyntax->Content = text.substr(1, text.size() - 2);
            }
            break;
        }
        case LuaSyntaxNodeKind::ClosureExpression: {
            auto closureExprSyntax = t.CreateSyntax<ClosureExprSyntax>(n);
            auto funcBody = n.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
            if (funcBody.IsNode(t)) {
                closureExprSyntax->ParamList = t.GetMember<ParamSyntaxList>(LuaSyntaxNodeKind::ParamList, funcBody);
                closureExprSyntax->Body = t.GetMember<BodySyntax>(LuaSyntaxNodeKind::Body, funcBody);
            }
            break;
        }
        case LuaSyntaxNodeKind::UnaryExpression: {
            auto unaryExprSyntax = t.CreateSyntax<UnaryExprSyntax>(n);
            auto op = n.GetFirstToken(t).GetTokenKind(t);
            unaryExprSyntax->UnaryOp = op;
            unaryExprSyntax->InnerExpr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::BinaryExpression: {
            auto binaryExprSyntax = t.CreateSyntax<BinaryExprSyntax>(n);
            auto op = n.GetChildToken(
                    [](auto kind) -> bool {
                        return LuaParser::GetBinaryOperator(kind) != BinOpr::OPR_NOBINOPR;
                    },
                    t);
            binaryExprSyntax->BinaryOp = op.GetTokenKind(t);
            auto exprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            if (!exprs.empty()) {
                binaryExprSyntax->LeftExpr = exprs[0];
            }
            if (exprs.size() == 2) {
                binaryExprSyntax->RightExpr = exprs[1];
            }
            break;
        }
        case LuaSyntaxNodeKind::TableExpression: {
            auto tableExprSyntax = t.CreateSyntax<TableExprSyntax>(n);
            auto tableExprList = n.GetChildSyntaxNode(LuaSyntaxNodeKind::TableFieldList, t);
            if (tableExprList.IsNode(t)) {
                tableExprSyntax->Fields = t.GetMembers<TableFieldSyntax>(LuaSyntaxNodeKind::TableField, tableExprList);
            }
            break;
        }
        case LuaSyntaxNodeKind::CallExpression: {
            auto callExprSyntax = t.CreateSyntax<CallExprSyntax>(n);
            callExprSyntax->Args = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::IndexExpression: {
            auto indexExprSyntax = t.CreateSyntax<IndexExprSyntax>(n);
            auto firstToken = n.GetFirstToken(t);
            indexExprSyntax->AccessToken = firstToken.GetTokenKind(t);
            indexExprSyntax->Expr = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::NameExpression: {
            auto nameExprSyntax = t.CreateSyntax<NameExprSyntax>(n);
            nameExprSyntax->Name = n.GetFirstToken(t).GetText(t);
            break;
        }
        case LuaSyntaxNodeKind::FunctionNameExpression: {
            auto funcNameExprSyntax = t.CreateSyntax<FuncNameExprSyntax>(n);
            funcNameExprSyntax->FirstName = n.GetChildToken(TK_NAME, t).GetText(t);
            funcNameExprSyntax->IndexExprs = t.GetMembers<IndexExprSyntax>(LuaSyntaxNodeKind::IndexExpression, n);
            break;
        }
        case LuaSyntaxNodeKind::VarList: {
            auto varList = t.CreateSyntax<VarSyntaxList>(n);
            varList->Vars = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        case LuaSyntaxNodeKind::TableField: {
            auto tableField = t.CreateSyntax<TableFieldSyntax>(n);
            auto op = n.GetChildToken(TK_EQ, t);
            if (op.IsToken(t)) {
                tableField->IsKeyValue = true;
                auto exprs = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
                if (exprs.size() == 2) {
                    tableField->Key = exprs[0];
                    tableField->Value = exprs[1];
                }
            } else {
                tableField->Value = t.GetMember<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            }
            break;
        }
        case LuaSyntaxNodeKind::ParamList: {
            auto paramSyntaxList = t.CreateSyntax<ParamSyntaxList>(n);
            auto paramTokens = n.GetChildTokens(TK_NAME, t);
            for (auto paramToken: paramTokens) {
                auto text = paramToken.GetText(t);
                paramSyntaxList->Params.emplace_back(text);
            }
            break;
        }
        case LuaSyntaxNodeKind::NameDefList: {
            auto nameDefSyntaxList = t.CreateSyntax<NameDefSyntaxList>(n);
            nameDefSyntaxList->List = t.GetMembers<NameDefSyntax>(LuaSyntaxNodeKind::NameDef, n);
            break;
        }
        case LuaSyntaxNodeKind::NameDef: {
            auto nameDefSyntax = t.CreateSyntax<NameDefSyntax>(n);
            nameDefSyntax->Name = n.GetChildToken(TK_NAME, t).GetText(t);
            auto attribute = n.GetChildSyntaxNode(LuaSyntaxNodeKind::Attribute, t);
            if (attribute.IsNode(t)) {
                auto attrName = attribute.GetChildToken(TK_NAME, t).GetText(t);
                nameDefSyntax->IsConst = attrName == "const";
                nameDefSyntax->IsClosed = attrName == "close";
            }
            break;
        }
        case LuaSyntaxNodeKind::ExpressionList: {
            auto exprSyntaxList = t.CreateSyntax<ExprSyntaxList>(n);
            exprSyntaxList->List = t.GetMembers<ExprSyntax>(LuaSyntaxMultiKind::Expression, n);
            break;
        }
        default: {
            break;
        }
    }
}
