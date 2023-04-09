#include "SymbolAnalyzer.h"
#include "Lua/SyntaxNode/Lua/LuaSyntaxs.h"
#include "Lua/SyntaxNode/LuaSyntaxNode.h"
#include "Lua/SyntaxNode/Visitor/LuaVisitor.h"
#include "Lua/SyntaxTree/LuaNodeOrToken.h"

using enum LuaTokenKind;

SymbolAnalyzer::SymbolAnalyzer() {
}

void SymbolAnalyzer::AnalyzeSymbol(const LuaSyntaxTree &t) {
    auto rootBody = t.GetRootNode();
    CreateScope();

    VisitBody(BodySyntax(rootBody), t);

    ExitScope();
}

void SymbolAnalyzer::CreateScope() {
}

void SymbolAnalyzer::ExitScope() {
}

void SymbolAnalyzer::VisitBody(BodySyntax body, const LuaSyntaxTree &t) {
    for (auto stmt: body.GetStmts(t)) {
        stmt.Accept(
            LuaVisitor{
                Visit<LocalStmtSyntax>([this](LocalStmtSyntax localStmtSyntax, auto &t) {
                    auto nameDefList = localStmtSyntax.GetNameDefList(t);
                    auto exprList = localStmtSyntax.GetExprList(t);

                    VisitNode(exprList, t);

                    auto names = nameDefList.GetNames(t);
                    for (auto name: names) {
                        //                    RecordLocalVariable(name, t);
                    }

                    auto exprs = exprList.GetExprList(t);
                    for (std::size_t i = 0; i != names.size(); i++) {
                        auto name = names[i];
                        bool matchSpecialRule = false;
                        if (i < exprs.size()) {
                            auto expr = exprs[i];
                            matchSpecialRule = CheckSpecialVariableRule(name, expr, t);
                        }
                        if (!matchSpecialRule) {
                            if (_scopeStack.size() == 1 && _module == name.GetText(t)) {
                                PushStyleCheck(NameDefineType::ModuleDefineName, name);
                                break;
                            }
                            PushStyleCheck(NameDefineType::LocalVariableName, name);
                        }
                    }
                })},
            t);
        switch (stmt.GetSyntaxKind(t)) {
            case LuaSyntaxNodeKind::AssignStatement: {
                auto varList = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::VarList, t);
                auto exprList = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::ExpressionList, t);
                CheckInNode(exprList, t);

                auto defineExprs = varList.GetChildSyntaxNodes(LuaSyntaxMultiKind::Expression, t);

                auto exprs = exprList.GetChildSyntaxNodes(LuaSyntaxMultiKind::Expression, t);
                for (std::size_t i = 0; i != defineExprs.size(); i++) {
                    auto defineExpr = defineExprs[i];
                    bool isNameDefine = defineExpr.GetSyntaxKind(t) == LuaSyntaxNodeKind::NameExpression;
                    if (isNameDefine) {
                        defineExpr = defineExpr.GetChildToken(TK_NAME, t);
                    } else {
                        // table field define
                        auto children = defineExpr.GetChildren(t);
                        if (!children.empty()) {
                            defineExpr = children.back().GetChildToken(TK_NAME, t);
                        }
                    }

                    bool matchSpecialRule = false;
                    if (i < exprs.size()) {
                        auto expr = exprs[i];
                        matchSpecialRule = CheckSpecialVariableRule(defineExpr, expr, t);
                    }

                    if (!matchSpecialRule) {
                        if (isNameDefine && CheckGlobal(defineExpr, t)) {
                            PushStyleCheck(NameDefineType::GlobalVariableDefineName, defineExpr);
                        } else if (!isNameDefine) {
                            PushStyleCheck(NameDefineType::TableFieldDefineName, defineExpr);
                        }
                    }
                }

                break;
            }
            case LuaSyntaxNodeKind::ForStatement: {
                auto forNumber = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::ForNumber, t);
                LuaSyntaxNode forBody(0);
                if (forNumber.IsNode(t)) {
                    for (auto expr: forNumber.GetChildSyntaxNodes(LuaSyntaxMultiKind::Expression, t)) {
                        CheckInNode(expr, t);
                    }

                    EnterScope();

                    auto indexName = forNumber.GetChildToken(TK_NAME, t);
                    RecordLocalVariable(indexName, t);
                    forBody = forNumber.GetChildSyntaxNode(LuaSyntaxNodeKind::ForBody, t);
                } else {
                    auto forList = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::ForList, t);
                    auto exprList = forList.GetChildSyntaxNode(LuaSyntaxNodeKind::ExpressionList, t);
                    for (auto expr: exprList.GetChildSyntaxNodes(LuaSyntaxMultiKind::Expression, t)) {
                        CheckInNode(expr, t);
                    }

                    EnterScope();

                    auto nameList = forList.GetChildSyntaxNode(LuaSyntaxNodeKind::NameDefList, t);
                    for (auto name: nameList.GetChildTokens(TK_NAME, t)) {
                        RecordLocalVariable(name, t);
                    }

                    forBody = forList.GetChildSyntaxNode(LuaSyntaxNodeKind::ForBody, t);
                }

                auto body = forBody.GetChildSyntaxNode(LuaSyntaxNodeKind::Block, t);
                CheckInBody(body, t);

                ExitScope();
                break;
            }
            case LuaSyntaxNodeKind::FunctionStatement: {
                auto functionNameExpr = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionNameExpression, t);
                LuaSyntaxNode functionName(0);
                bool simpleFunction = false;
                auto indexExpr = functionNameExpr.GetChildSyntaxNode(LuaSyntaxNodeKind::IndexExpression, t);
                if (indexExpr.IsNode(t)) {
                    auto indexExprs = functionNameExpr.GetChildSyntaxNodes(LuaSyntaxNodeKind::IndexExpression, t);
                    functionName = indexExprs.back().GetChildToken(TK_NAME, t);
                } else {
                    functionName = functionNameExpr.GetChildToken(TK_NAME, t);
                    simpleFunction = true;
                }

                PushStyleCheck(NameDefineType::FunctionDefineName, functionName);
                EnterScope();
                // for recursive
                if (simpleFunction) {
                    RecordLocalVariable(functionName, t);
                }

                auto functionBody = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
                auto paramList = functionBody.GetChildSyntaxNode(LuaSyntaxNodeKind::ParamList, t);
                auto params = paramList.GetChildTokens(TK_NAME, t);
                for (auto paramName: params) {
                    RecordLocalVariable(paramName, t);
                    PushStyleCheck(NameDefineType::ParamName, paramName);
                }

                auto body = functionBody.GetChildSyntaxNode(LuaSyntaxNodeKind::Block, t);
                CheckInBody(body, t);

                ExitScope();
                break;
            }
            case LuaSyntaxNodeKind::LocalFunctionStatement: {
                auto functionName = stmt.GetChildToken(TK_NAME, t);
                PushStyleCheck(NameDefineType::LocalFunctionName, functionName);
                EnterScope();
                // for recursive
                RecordLocalVariable(functionName, t);

                auto functionBody = stmt.GetChildSyntaxNode(LuaSyntaxNodeKind::FunctionBody, t);
                auto paramList = functionBody.GetChildSyntaxNode(LuaSyntaxNodeKind::ParamList, t);
                auto params = paramList.GetChildTokens(TK_NAME, t);
                for (auto paramName: params) {
                    RecordLocalVariable(paramName, t);
                    PushStyleCheck(NameDefineType::ParamName, paramName);
                }

                auto body = functionBody.GetChildSyntaxNode(LuaSyntaxNodeKind::Block, t);
                CheckInBody(body, t);

                ExitScope();
                break;
            }
            default: {
                CheckInNode(stmt, t);
                break;
            }
        }
    }
}

void SymbolAnalyzer::VisitNode(LuaSyntaxNode n, const LuaSyntaxTree &t) {
}
