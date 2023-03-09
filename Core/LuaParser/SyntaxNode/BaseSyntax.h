#pragma once

#include "LuaSyntaxNode.h"

class BaseSyntax {
public:
    explicit BaseSyntax(LuaSyntaxNode n);
    virtual ~BaseSyntax();

    LuaSyntaxNode GetSyntaxNode() const;

    LuaSyntaxNodeKind GetKind(const LuaSyntaxTree &t) const;

    template<class SyntaxClass>
    bool Is() {
        return dynamic_cast<SyntaxClass *>(this) != nullptr;
    }

    template<class SyntaxClass>
    SyntaxClass *As() {
        return dynamic_cast<SyntaxClass *>(this);
    }

    void Accept(class LuaVisitor& visitor, const LuaSyntaxTree& t);
protected:
    LuaSyntaxNode _node;
};
