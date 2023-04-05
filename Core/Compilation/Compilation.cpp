#include "Compilation.h"

Compilation::Compilation()
    : _idCounter(0) {
}

Compilation::TreeId Compilation::AddSyntaxTree(const LuaSyntaxTree &t) {

}

void Compilation::RemoveSyntaxTree(Compilation::TreeId treeId) {
}

SemanticModel Compilation::GetSemanticModel(Compilation::TreeId treeId) {
    return SemanticModel(this);
}

Compilation::TreeId Compilation::AllocId() {
    return _idCounter++;
}
