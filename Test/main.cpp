#include "LuaParser/Ast/LuaSyntaxTree.h"
#include "LuaParser/Ast/LuaTreeBuilder.h"
#include "LuaParser/DocLexer/LuaDocLexer.h"
#include "LuaParser/Lexer/LuaLexer.h"
#include "LuaParser/Parser/LuaParser.h"
#include "fmt/printf.h"
#include <fmt/format.h>
#include <iostream>

int main() {
    std::string s = R"(---@generic k: OK<SSS>, V, Y: DDD)";

    LuaFile luaFile;
    luaFile.UpdateFile(std::move(s));

    //    LuaLexer luaLexer(file);
    //    auto &tokens = luaLexer.Tokenize();
    //
    //    LuaParser p(file, std::move(tokens));
    //    p.Parse();
    //
    //    LuaSyntaxTree t(file);
    //
    //    LuaTreeBuilder treeBuilder;
    //    treeBuilder.BuildTree(t, p);
    //
    ////    fmt::printf("{}", t.GetDebugView());
    //    std::cout << t.GetDebugView() <<std::endl;

    LuaDocLexer l(luaFile.GetSource(), 0);
    auto t = l.Tokenize();

    return 0;
}