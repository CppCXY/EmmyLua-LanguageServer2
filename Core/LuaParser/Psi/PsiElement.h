#pragma once

#include "LuaParser/Ast/LuaSyntaxNode.h"
#include "LuaParser/Ast/LuaSyntaxTree.h"
#include "PsiType.h"
#include <algorithm>

#define PSI_RTTI                                                                                            \
    static bool __RTTI__(LuaSyntaxNode n, LuaSyntaxTree &t) {                                               \
        auto kind = n.GetSyntaxKind(t);                                                                     \
        return std::any_of(__RTTI__DATA.begin(), __RTTI__DATA.end(), [kind](auto x) { return x == kind; }); \
    }                                                                                                       \
                                                                                                            \
    inline static std::vector<LuaSyntaxNodeKind> __RTTI__DATA =

#define PSI_RTTI_ANY                                          \
    static bool __RTTI__(LuaSyntaxNode n, LuaSyntaxTree &t) { \
        return true;                                          \
    }

class PsiElement {
public:
    PSI_RTTI_ANY

    explicit PsiElement(LuaSyntaxNode n);

    template<class Psi>
    bool Is(LuaSyntaxTree &t) const {
        return Psi::__RTTI__(_node, t);
    }

    template<class Psi>
    Psi Cast(LuaSyntaxTree &t) const {
        if (Is<Psi>(t)) {
            return Psi(_node);
        }
        return Psi(LuaSyntaxNode());
    }

private:
    LuaSyntaxNode _node;
};
