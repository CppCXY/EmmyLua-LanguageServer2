#pragma once

#include "LuaParser/SyntaxNode/LuaSyntaxNode.h"
#include "LuaParser/SyntaxTree/LuaSyntaxTree.h"

template<class Psi>
struct PsiCaster {
    static bool CanCast(LuaSyntaxNode n, const LuaSyntaxTree &t) { return true; }

    static Psi Cast(LuaSyntaxNode n, const LuaSyntaxTree &t) {
        if (PsiCaster<Psi>::CanCast(n, t)) {
            return Psi(n);
        }
        return Psi(LuaSyntaxNode());
    }
};