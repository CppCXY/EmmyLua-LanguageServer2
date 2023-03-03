#include "BaseSyntax.h"

BaseSyntax::BaseSyntax(LuaSyntaxNode n)
    : _node(n) {
}

LuaSyntaxNode BaseSyntax::GetSyntaxNode() const {
    return _node;
}

BaseSyntax::~BaseSyntax() {
}
