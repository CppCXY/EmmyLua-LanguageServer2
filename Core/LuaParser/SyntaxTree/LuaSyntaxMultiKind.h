#pragma once

#include "LuaParser/Kind/LuaSyntaxNodeKind.h"

enum class LuaSyntaxMultiKind {
    Expression,
    Statement,
};

// try detail::multi_match
namespace detail {
namespace multi_match {
bool Match(LuaSyntaxMultiKind k, LuaSyntaxNodeKind syntaxNodeKind);
bool ExpressionMatch(LuaSyntaxNodeKind k);
bool StatementMatch(LuaSyntaxNodeKind k);
}// namespace multi_match
}// namespace detail