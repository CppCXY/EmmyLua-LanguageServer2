#include "BodySyntax.h"
#include "Lua/SyntaxNode/Doc/CommentSyntax.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"
#include "StmtSyntax.h"

BodySyntax::BodySyntax(LuaNodeOrToken n) : LuaBaseSyntax(n) {
}

std::vector<StmtSyntax> BodySyntax::GetStmts(const LuaSyntaxTree &t) {
    return GetMembers<StmtSyntax>(t);
}

std::vector<struct CommentSyntax> BodySyntax::GetComments(const LuaSyntaxTree &t) {
    return GetMembers<CommentSyntax>(t);
}
