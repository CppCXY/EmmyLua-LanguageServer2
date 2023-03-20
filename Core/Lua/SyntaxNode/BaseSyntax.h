#pragma once

#include "Lua/SyntaxTree/LuaNodeOrToken.h"

class BaseSyntax {
public:
    explicit BaseSyntax(LuaNodeOrToken n);
    virtual ~BaseSyntax();

    LuaNodeOrToken GetNode() const;

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
    LuaNodeOrToken _node;
};
