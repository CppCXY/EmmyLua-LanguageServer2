#pragma once

#include "Compilation/SemanticModel/SemanticModel.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"

class Compilation {
public:
    using TreeId = std::size_t;

    Compilation();

    TreeId AddSyntaxTree(const LuaSyntaxTree &t);

    void RemoveSyntaxTree(TreeId treeId);

    SemanticModel GetSemanticModel(TreeId treeId);

private:
    TreeId AllocId();

    TreeId _idCounter;
};
