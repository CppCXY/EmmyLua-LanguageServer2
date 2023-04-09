#include "SyntaxNodeIterator.h"

SyntaxNodeIterator::SyntaxNodeIterator(LuaNodeOrToken n)
    : _n(n) {
}

LuaNodeOrToken SyntaxNodeIterator::Get(const LuaSyntaxTree &t) {
    return _n;
}

void SyntaxNodeIterator::Next(const LuaSyntaxTree &t) {
    return _n.ToNext(t);
}

bool SyntaxNodeIterator::IsValid(const LuaSyntaxTree &t) {
    return !_n.IsNull(t);
}
