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

        switch (syntax->GetKind(t)) {
            case LuaSyntaxNodeKind::Body: {
                VisitSyntax(BodySyntax);
                break;
            }
            case LuaSyntaxNodeKind::LocalStatement: {
                VisitSyntax(LocalStmtSyntax);
                break;
            }
            case LuaSyntaxNodeKind::LocalFunctionStatement: {
                VisitSyntax(LocalFuncStmtSyntax) break;
            }
                // TODO
            default: {
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
}

void LuaVisitor::VisitLocalStmtSyntax(const LocalStmtSyntax *body, const LuaSyntaxTree &t) {
}

void LuaVisitor::VisitLocalFuncStmtSyntax(const LocalFuncStmtSyntax *body, const LuaSyntaxTree &t) {
}
