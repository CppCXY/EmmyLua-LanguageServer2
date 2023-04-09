#pragma once

#include "Compilation/SemanticModel/SemanticModel.h"
#include "Compilation/Symbol/SymbolTable.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"

class Compilation {
public:
    using CompilationUnitId = std::size_t;

    Compilation();

    CompilationUnitId AddSyntaxTree(const LuaSyntaxTree &t);

    void RemoveSyntaxTree(CompilationUnitId id);

    SemanticModel GetSemanticModel(CompilationUnitId id);

private:
    CompilationUnitId AllocId();

    CompilationUnitId _idCounter;
    std::unordered_map<CompilationUnitId, SymbolTable> _symbolTables;
};
