
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
auto benchmark(F func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    std::string s = R"(
local t = 123
)";
    std::fstream f("C:\\Users\\zc\\Desktop\\github\\EmmyLua-LanguageServer2\\Test\\10k_row_code.lua");
    std::string text;
    if (f.is_open()) {
        std::stringstream s;
        s << f.rdbuf();
        text = s.str();
    }

    auto t = LuaSyntaxTree::ParseText(std::move(text));

    auto r = t->GetRootNode();
    LuaSyntaxNode root(r);
    auto t1 = benchmark([&]() {
        root.Accept(
            LuaVisitor{
                Visit<LocalStmtSyntax>([](LocalStmtSyntax n, auto &t) {
                    volatile auto l = n.GetNode().GetTokenKind(t);
//                    std::cout << n.GetNode().GetText(t) << std::endl;
                }),
                Visit<NameDefSyntaxList>([](NameDefSyntaxList n, auto &t) {
                    volatile auto l = n.GetNode().GetTokenKind(t);
//                    std::cout << n.GetNode().GetText(t) << std::endl;
                })},
            *t);
    });

    auto t2 = benchmark([&]() {
        std::stack<LuaNodeOrToken> traverseStack;
        traverseStack.push(r);
        // 非递归深度优先遍历
        while (!traverseStack.empty()) {
            LuaNodeOrToken n = traverseStack.top();
            traverseStack.pop();

            switch (n.GetSyntaxKind(*t)) {
                case LuaSyntaxNodeKind::LocalStatement: {
                    volatile auto l = LocalStmtSyntax(n);
                    break;
                }
                case LuaSyntaxNodeKind::NameDefList: {
                    volatile auto l = NameDefSyntaxList(n);
                    break;
                }
                default: {
                    break;
                }
            }

            if (n.IsNode(*t)) {
                auto children = n.GetChildren(*t);
                for (auto &c: children | std::views::reverse) {
                    traverseStack.push(c);
                }
            }
        }
    });

    std::cout << "time 1" << t1 << std::endl;
    std::cout << "time 2" << t2 << std::endl;
    return 0;
}