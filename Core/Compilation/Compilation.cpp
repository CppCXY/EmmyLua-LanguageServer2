#include "Compilation.h"
#include "Compilation/Symbol/SymbolAnalyzer.h"

Compilation::Compilation()
    : _idCounter(0) {
}

Compilation::CompilationUnitId Compilation::AddSyntaxTree(const LuaSyntaxTree &t) {
    SymbolAnalyzer symbolAnalyzer;
    symbolAnalyzer.AnalyzeSymbol(t);
}

void Compilation::RemoveSyntaxTree(Compilation::CompilationUnitId id) {
}

SemanticModel Compilation::GetSemanticModel(Compilation::CompilationUnitId id) {
    return SemanticModel(this);
}

Compilation::CompilationUnitId Compilation::AllocId() {
    return _idCounter++;
}
