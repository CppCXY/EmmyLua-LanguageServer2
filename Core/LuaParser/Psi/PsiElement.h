#pragma once

#include "LuaParser/Ast/LuaSyntaxNode.h"
#include "LuaParser/Ast/LuaSyntaxTree.h"
#include "PsiCaster.h"
#include "PsiType.h"
#include <algorithm>

class PsiElement {
public:
    explicit PsiElement(LuaSyntaxNode n);

    template<class Psi>
    bool Is(LuaSyntaxTree &t) const {
        return PsiCaster<Psi>::CanCast(_node, t);
    }

    template<class Psi>
    Psi Cast(LuaSyntaxTree &t) const {
        return PsiCaster<Psi>::Cast(_node, t);
    }

private:
    LuaSyntaxNode _node;
};

template <>
struct PsiCaster<PsiElement> {
    static bool CanCast(LuaSyntaxNode n, const LuaSyntaxTree &t) { return true; }
};
