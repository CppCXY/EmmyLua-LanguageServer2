#include "LuaSyntaxNode.h"
#include "Visitor/LuaVisitor.h"

LuaSyntaxNode::LuaSyntaxNode(LuaNodeOrToken n)
    : _node(n) {
}

LuaNodeOrToken LuaSyntaxNode::GetNode() const {
    return _node;
}

LuaSyntaxNode::~LuaSyntaxNode() {
}

LuaSyntaxNodeKind LuaSyntaxNode::GetKind(const LuaSyntaxTree &t) const {
    return _node.GetSyntaxKind(t);
}

void LuaSyntaxNode::Accept(LuaVisitor &visitor, const LuaSyntaxTree &t) {
    this.Descendants()
        .Visit<BodySYntax>([](auto b, auto& t) {

            });
}
