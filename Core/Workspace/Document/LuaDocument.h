#pragma once

#include "LuaParser/SyntaxTree/LuaSyntaxTree.h"

class LuaDocument {
public:
    using DocumentId = std::size_t;

    explicit LuaDocument(DocumentId id = 0);

    LuaSyntaxTree & GetSyntaxTree();

private:
    DocumentId _id;
};

