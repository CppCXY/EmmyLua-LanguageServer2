#include "VarSyntaxList.h"

VarSyntaxList::VarSyntaxList(LuaNodeOrToken n) : LuaBaseSyntax(n) {
}

std::vector<ExprSyntax> VarSyntaxList::GetVars(const LuaSyntaxTree &t) const {
    return GetMember<ExprSyntax>(t);
}
