
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
#include <fstream>
#include <iostream>

template<class F>
double benchmark(F func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    std::string s = R"(
local t = 123
)";

    auto t = LuaSyntaxTree::ParseText(std::move(s));

    auto r = t->GetRootNode();
    LuaSyntaxNode root(r);

    return 0;
}