#pragma once

#include "Lua/SyntaxNode/Lua/BodySyntax.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"
#include "SymbolTable.h"

class SymbolAnalyzer {
public:
    SymbolAnalyzer();

    void AnalyzeSymbol(const LuaSyntaxTree &t);

private:
    void CreateScope();

    void ExitScope();

    void VisitBody(BodySyntax body, const LuaSyntaxTree &t);

    void VisitNode(LuaSyntaxNode n, const LuaSyntaxTree &t);

    SymbolTable _symbolTable;
};
