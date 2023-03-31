#include "LuaVisitor.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"
#include <ranges>

LuaVisitor::LuaVisitor() {
}

void LuaVisitor::Visit(LuaSyntaxNode &syntax, const LuaSyntaxTree &t) {
    if (syntax.GetNode().IsNull(t)) {
        return;
    }

}

