
#include "Lua/DocLexer/LuaDocLexer.h"
#include "Lua/LuaLexer/LuaLexer.h"
#include "Lua/LuaParser/LuaParser.h"
#include "Lua/Source/LuaSource.h"
#include "Lua/SyntaxNode/Lua/LuaSyntaxs.h"
#include "Lua/SyntaxNode/LuaSyntaxNode.h"
#include "Lua/SyntaxNode/Visitor/LuaVisitor.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"
#include "fmt/printf.h"
#include <fmt/format.h>
#include <iostream>

int main() {
    std::string s = R"(
local t = 123
)";

    auto t = LuaSyntaxTree::ParseText(std::move(s));

    auto r = t->GetRootNode();
    LuaSyntaxNode root(r);
    root.Accept(
            LuaVisitor{
                    Visit<LocalStmtSyntax>([](LocalStmtSyntax n, auto &t) {
                        std::cout << n.GetNode().GetText(t) << std::endl;
                    }),
                    Visit<NameDefSyntaxList>([](NameDefSyntaxList n, auto &t) {
                        std::cout << n.GetNode().GetText(t) << std::endl;
                    })},
            *t);

    return 0;
}