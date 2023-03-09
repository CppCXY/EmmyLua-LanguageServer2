#include "LuaParser/SyntaxNode/BaseSyntax.h"
#include "LuaParser/SyntaxNode/Lua/LuaSyntaxs.h"

class LuaVisitor {
public:
    LuaVisitor();

    void Visit(BaseSyntax *syntax, const LuaSyntaxTree &t);

protected:
    virtual void VisitBodySyntax(const BodySyntax *body, const LuaSyntaxTree &t);

    virtual void VisitLocalStmtSyntax(const LocalStmtSyntax *body, const LuaSyntaxTree &t);

    virtual void VisitLocalFuncStmtSyntax(const LocalFuncStmtSyntax *body, const LuaSyntaxTree &t);
    //TODO
};
