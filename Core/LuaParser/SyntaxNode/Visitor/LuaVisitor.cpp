#include "LuaVisitor.h"
#include "LuaParser/SyntaxTree/LuaSyntaxTree.h"
#include <ranges>

LuaVisitor::LuaVisitor() {
}

void LuaVisitor::Visit(BaseSyntax *syntax, const LuaSyntaxTree &t) {
    if (!syntax) {
        return;
    }

    std::stack<LuaSyntaxNode> traverseStack;
    traverseStack.push(syntax->GetSyntaxNode());
    // 非递归深度优先遍历
    while (!traverseStack.empty()) {
        LuaSyntaxNode n = traverseStack.top();
        traverseStack.pop();

#define VisitSyntax(SyntaxType)            \
    auto ptr = t.GetSyntax<SyntaxType>(n); \
    if (ptr) { Visit##SyntaxType(ptr, t); }

        switch (n.GetSyntaxKind(t)) {
            case LuaSyntaxNodeKind::Body: {
                VisitSyntax(BodySyntax);
                break;
            }
            case LuaSyntaxNodeKind::LocalStatement: {
                VisitSyntax(LocalStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::LocalFunctionStatement: {
                VisitSyntax(LocalFuncStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::AssignStatement: {
                VisitSyntax(AssignStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::BreakStatement: {
                VisitSyntax(BreakStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::DoStatement: {
                VisitSyntax(DoStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::IfStatement: {
                VisitSyntax(CallStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::FunctionStatement: {
                VisitSyntax(FuncStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::LabelStatement: {
                VisitSyntax(LabelStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::WhileStatement: {
                VisitSyntax(WhileStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::ForStatement: {
                VisitSyntax(ForStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::GotoStatement: {
                VisitSyntax(RepeatStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::BinaryExpression: {
                VisitSyntax(BinaryExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::UnaryExpression: {
                VisitSyntax(UnaryExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::CallExpression: {
                VisitSyntax(CallExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::ClosureExpression: {
                VisitSyntax(ClosureExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::LiteralExpression: {
                VisitSyntax(LiteralExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::StringLiteralExpression: {
                VisitSyntax(StringLiteralExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::IndexExpression: {
                VisitSyntax(IndexExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::NameExpression: {
                VisitSyntax(NameExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::FunctionNameExpression: {
                VisitSyntax(FuncNameExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::ParExpression: {
                VisitSyntax(ParExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::SuffixedExpression: {
                VisitSyntax(SuffixedExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::TableExpression: {
                VisitSyntax(TableExprSyntax);
                break;
            }
            case LuaSyntaxNodeKind::ExpressionList: {
                VisitSyntax(ExprSyntaxList);
                break;
            }
            case LuaSyntaxNodeKind::ParamList: {
                VisitSyntax(ParamSyntaxList);
                break;
            }
            case LuaSyntaxNodeKind::NameDefList: {
                VisitSyntax(NameDefSyntaxList);
                break;
            }
            case LuaSyntaxNodeKind::VarList: {
                VisitSyntax(VarSyntaxList);
                break;
            }
            case LuaSyntaxNodeKind::NameDef: {
                VisitSyntax(NameDefSyntax);
                break;
            }
            case LuaSyntaxNodeKind::TableField: {
                VisitSyntax(TableFieldSyntax);
                break;
            }
            case LuaSyntaxNodeKind::Comment: {
                VisitSyntax(CommentSyntax);
                break;
            }
            default: {
                VisitSyntax(BaseSyntax);
                break;
            }
        }

        if (n.IsNode(t)) {
            auto children = n.GetChildren(t);
            for (auto &c: children | std::views::reverse) {
                traverseStack.push(c);
            }
        }
    }
}

void LuaVisitor::VisitBodySyntax(const BodySyntax *body, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(body, t);
}

void LuaVisitor::VisitStmtSyntax(const StmtSyntax *stmtSyntax, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(stmtSyntax, t);
}

void LuaVisitor::VisitLocalStmtSyntax(const LocalStmtSyntax *localStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(localStmtSyntax, t);
}

void LuaVisitor::VisitLocalFuncStmtSyntax(const LocalFuncStmtSyntax *localFuncStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(localFuncStmtSyntax, t);
}

void LuaVisitor::VisitAssignStmtSyntax(const AssignStmtSyntax *assignStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(assignStmtSyntax, t);
}

void LuaVisitor::VisitBreakStmtSyntax(const BreakStmtSyntax *breakStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(breakStmtSyntax, t);
}

void LuaVisitor::VisitDoStmtSyntax(const DoStmtSyntax *doStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(doStmtSyntax, t);
}

void LuaVisitor::VisitIfStmtSyntax(const IfStmtSyntax *ifStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(ifStmtSyntax, t);
}

void LuaVisitor::VisitCallStmtSyntax(const CallStmtSyntax *callStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(callStmtSyntax, t);
}

void LuaVisitor::VisitFuncStmtSyntax(const FuncStmtSyntax *funcStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(funcStmtSyntax, t);
}

void LuaVisitor::VisitLabelStmtSyntax(const LabelStmtSyntax *labelStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(labelStmtSyntax, t);
}

void LuaVisitor::VisitWhileStmtSyntax(const WhileStmtSyntax *whileStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(whileStmtSyntax, t);
}

void LuaVisitor::VisitForStmtSyntax(const ForStmtSyntax *forStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(forStmtSyntax, t);
}

void LuaVisitor::VisitGotoStmtSyntax(const GotoStmtSyntax *gotoStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(gotoStmtSyntax, t);
}

void LuaVisitor::VisitRepeatStmtSyntax(const RepeatStmtSyntax *repeatStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(repeatStmtSyntax, t);
}

void LuaVisitor::VisitReturnStmtSyntax(const ReturnStmtSyntax *returnStmtSyntax, const LuaSyntaxTree &t) {
    return VisitStmtSyntax(returnStmtSyntax, t);
}

void LuaVisitor::VisitExprSyntax(const ExprSyntax *exprSyntax, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(exprSyntax, t);
}

void LuaVisitor::VisitBinaryExprSyntax(const BinaryExprSyntax *binaryExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(binaryExprSyntax, t);
}

void LuaVisitor::VisitUnaryExprSyntax(const UnaryExprSyntax *unaryExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(unaryExprSyntax, t);
}

void LuaVisitor::VisitCallExprSyntax(const CallExprSyntax *callExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(callExprSyntax, t);
}

void LuaVisitor::VisitClosureExprSyntax(const ClosureExprSyntax *closureExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(closureExprSyntax, t);
}

void LuaVisitor::VisitLiteralExprSyntax(const LiteralExprSyntax *literalExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(literalExprSyntax, t);
}

void LuaVisitor::VisitStringLiteralExprSyntax(const StringLiteralExprSyntax *stringLiteralExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(stringLiteralExprSyntax, t);
}

void LuaVisitor::VisitIndexExprSyntax(const IndexExprSyntax *indexExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(indexExprSyntax, t);
}

void LuaVisitor::VisitNameExprSyntax(const NameExprSyntax *nameExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(nameExprSyntax, t);
}

void LuaVisitor::VisitFuncNameExprSyntax(const FuncNameExprSyntax *funcNameExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(funcNameExprSyntax, t);
}

void LuaVisitor::VisitParExprSyntax(const ParExprSyntax *parExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(parExprSyntax, t);
}

void LuaVisitor::VisitSuffixedExprSyntax(const SuffixedExprSyntax *suffixedExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(suffixedExprSyntax, t);
}

void LuaVisitor::VisitTableExprSyntax(const TableExprSyntax *tableExprSyntax, const LuaSyntaxTree &t) {
    return VisitExprSyntax(tableExprSyntax, t);
}

void LuaVisitor::VisitExprSyntaxList(const ExprSyntaxList *exprSyntaxList, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(exprSyntaxList, t);
}

void LuaVisitor::VisitParamSyntaxList(const ParamSyntaxList *paramSyntaxList, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(paramSyntaxList, t);
}

void LuaVisitor::VisitNameDefSyntaxList(const NameDefSyntaxList *nameDefSyntaxList, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(nameDefSyntaxList, t);
}

void LuaVisitor::VisitVarSyntaxList(const VarSyntaxList *varSyntaxList, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(varSyntaxList, t);
}

void LuaVisitor::VisitNameDefSyntax(const NameDefSyntax *nameDefSyntax, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(nameDefSyntax, t);
}

void LuaVisitor::VisitTableFieldSyntax(const TableFieldSyntax *tableFieldSyntax, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(tableFieldSyntax, t);
}

void LuaVisitor::VisitCommentSyntax(const CommentSyntax *commentSyntax, const LuaSyntaxTree &t) {
    return VisitBaseSyntax(commentSyntax, t);
}

void LuaVisitor::VisitBaseSyntax(const BaseSyntax *baseSyntax, const LuaSyntaxTree &t) {
}
