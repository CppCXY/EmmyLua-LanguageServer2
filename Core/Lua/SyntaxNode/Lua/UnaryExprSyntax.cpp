#include "UnaryExprSyntax.h"

UnaryExprSyntax::UnaryExprSyntax(LuaNodeOrToken n) : LuaBaseSyntax(n) {
}

ExprSyntax UnaryExprSyntax::GetInnerExpr(const LuaSyntaxTree &t) const {
    return GetMember<ExprSyntax>(t);
}
