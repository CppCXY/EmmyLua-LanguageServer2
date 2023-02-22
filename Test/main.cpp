#include "LuaParser/Ast/LuaSyntaxTree.h"
#include "LuaParser/Ast/LuaTreeBuilder.h"
#include "LuaParser/Lexer/LuaLexer.h"
#include "LuaParser/Parser/LuaParser.h"
#include "fmt/printf.h"
#include <fmt/format.h>
#include <iostream>

int main() {
    std::string s = R"(


----a a a

---uuuu
)";

    auto file = std::make_shared<LuaFile>(std::move(s));
    file->BuildLineIndex();

    LuaLexer luaLexer(file);
    auto &tokens = luaLexer.Tokenize();

    LuaParser p(file, std::move(tokens));
    p.Parse();

    LuaSyntaxTree t(file);

    LuaTreeBuilder treeBuilder;
    treeBuilder.BuildTree(t, p);

//    fmt::printf("{}", t.GetDebugView());
    std::cout << t.GetDebugView() <<std::endl;

    return 0;
}