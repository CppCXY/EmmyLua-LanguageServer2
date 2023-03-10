
#include "LuaParser/DocLexer/LuaDocLexer.h"
#include "LuaParser/Lexer/LuaLexer.h"
#include "LuaParser/Parser/LuaParser.h"
#include "LuaParser/Source/LuaSource.h"
#include "LuaParser/SyntaxNode/Visitor/LuaVisitor.h"
#include "LuaParser/SyntaxTree/LuaSyntaxTree.h"
#include "fmt/printf.h"
#include <fmt/format.h>
#include <iostream>

int main() {
    std::string s = R"(

---@class Hello

---@generic T: number
---@class MyClass<T>
local t = 123

---@param a T
---@return T
function t:f(a)
end

)";

    auto t = LuaSyntaxTree::ParseText(std::move(s));

    std::cout << t.GetDebugSyntaxView() << std::endl;


    return 0;
}