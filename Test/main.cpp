
#include "Lua/DocLexer/LuaDocLexer.h"
#include "Lua/LuaLexer/LuaLexer.h"
#include "Lua/LuaParser/LuaParser.h"
#include "Lua/Source/LuaSource.h"
#include "Lua/SyntaxNode/Visitor/LuaVisitor.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"
#include "fmt/printf.h"
#include <fmt/format.h>
#include <iostream>

int main() {
    std::string s = R"(
---@class A : B
local t = 123

function t:f(a)
end

)";

    auto t = LuaSyntaxTree::ParseText(std::move(s));

    std::cout << t.GetDebugView() << std::endl;

    std::cout << t.GetDebugSyntaxView() << std::endl;

    return 0;
}