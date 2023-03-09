#include "BaseSyntax.h"
#include "Visitor/LuaVisitor.h"

BaseSyntax::BaseSyntax(LuaSyntaxNode n)
    : _node(n) {
}

LuaSyntaxNode BaseSyntax::GetSyntaxNode() const {
    return _node;
}

BaseSyntax::~BaseSyntax() {
}

LuaSyntaxNodeKind BaseSyntax::GetKind(const LuaSyntaxTree &t) const {
    return _node.GetSyntaxKind(t);
}

void BaseSyntax::Accept(LuaVisitor &visitor, const LuaSyntaxTree &t) {
    visitor.Visit(this, t);
}
