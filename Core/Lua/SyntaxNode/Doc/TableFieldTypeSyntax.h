#pragma once

#include "DocBaseSyntax.h"

enum class TableFieldKey {
    None,
    Name,
    Type
};

class TableFieldTypeSyntax : public DocBaseSyntax {
public:
    TableFieldTypeSyntax(LuaNodeOrToken n);

    std::string_view GetStringKey(const LuaSyntaxTree& t);

    int64_t GetIndexKey(const LuaSyntaxTree& t);

    TableFieldKey KeyType = TableFieldKey::None;
    class TypeSyntax *Type = nullptr;
};
