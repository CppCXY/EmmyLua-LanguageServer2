#include "Lua/SyntaxNode/Lua/LuaSyntaxs.h"
#include "Lua/SyntaxNode/LuaSyntaxNode.h"

class LuaVisitor {
public:
    LuaVisitor();

    void Visit(LuaSyntaxNode *syntax, const LuaSyntaxTree &t);

protected:
    virtual void VisitBodySyntax(const BodySyntax *body, const LuaSyntaxTree &t);

    // stmt
    virtual void VisitStmtSyntax(const StmtSyntax *stmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitLocalStmtSyntax(const LocalStmtSyntax *localStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitLocalFuncStmtSyntax(const LocalFuncStmtSyntax *localFuncStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitAssignStmtSyntax(const AssignStmtSyntax *assignStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitBreakStmtSyntax(const BreakStmtSyntax *breakStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitDoStmtSyntax(const DoStmtSyntax *doStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitIfStmtSyntax(const IfStmtSyntax *ifStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitCallStmtSyntax(const CallStmtSyntax *callStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitFuncStmtSyntax(const FuncStmtSyntax *funcStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitLabelStmtSyntax(const LabelStmtSyntax *labelStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitWhileStmtSyntax(const WhileStmtSyntax *whileStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitForStmtSyntax(const ForNumberStmtSyntax *forStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitGotoStmtSyntax(const GotoStmtSyntax *gotoStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitRepeatStmtSyntax(const RepeatStmtSyntax *repeatStmtSyntax, const LuaSyntaxTree &t);

    virtual void VisitReturnStmtSyntax(const ReturnStmtSyntax *returnStmtSyntax, const LuaSyntaxTree &t);

    // expr
    virtual void VisitExprSyntax(const ExprSyntax *exprSyntax, const LuaSyntaxTree &t);

    virtual void VisitBinaryExprSyntax(const BinaryExprSyntax *binaryExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitUnaryExprSyntax(const UnaryExprSyntax *unaryExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitCallExprSyntax(const CallExprSyntax *callExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitClosureExprSyntax(const ClosureExprSyntax *closureExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitLiteralExprSyntax(const LiteralExprSyntax *literalExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitStringLiteralExprSyntax(const StringLiteralExprSyntax *stringLiteralExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitIndexExprSyntax(const IndexExprSyntax *indexExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitNameExprSyntax(const NameExprSyntax *nameExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitFuncNameExprSyntax(const FuncNameExprSyntax *funcNameExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitParExprSyntax(const ParExprSyntax *parExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitSuffixedExprSyntax(const SuffixedExprSyntax *suffixedExprSyntax, const LuaSyntaxTree &t);

    virtual void VisitTableExprSyntax(const TableExprSyntax *tableExprSyntax, const LuaSyntaxTree &t);

    // list

    virtual void VisitExprSyntaxList(const ExprSyntaxList *exprSyntaxList, const LuaSyntaxTree &t);

    virtual void VisitParamSyntaxList(const ParamSyntaxList *paramSyntaxList, const LuaSyntaxTree &t);

    virtual void VisitNameDefSyntaxList(const NameDefSyntaxList *nameDefSyntaxList, const LuaSyntaxTree &t);

    virtual void VisitVarSyntaxList(const VarSyntaxList *varSyntaxList, const LuaSyntaxTree &t);

    // other

    virtual void VisitNameDefSyntax(const NameDefSyntax *nameDefSyntax, const LuaSyntaxTree &t);

    virtual void VisitTableFieldSyntax(const TableFieldSyntax *tableFieldSyntax, const LuaSyntaxTree &t);

    virtual void VisitCommentSyntax(const CommentSyntax *commentSyntax, const LuaSyntaxTree &t);

    // final access
    virtual void VisitBaseSyntax(const LuaSyntaxNode *baseSyntax, const LuaSyntaxTree &t);
};
