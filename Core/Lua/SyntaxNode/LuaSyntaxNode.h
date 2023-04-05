#pragma once

#include "Lua/SyntaxTree/LuaNodeOrToken.h"

class LuaSyntaxNode {
public:
    static bool CanCast(LuaSyntaxNodeKind kind) {
        return true;
    }

    explicit LuaSyntaxNode(LuaNodeOrToken n);

    LuaNodeOrToken GetNode() const;

    LuaSyntaxNodeKind GetKind(const LuaSyntaxTree &t) const;

    template<class SyntaxClass>
    SyntaxClass Cast() {
        return SyntaxClass(_node);
    }

    template<class SyntaxClass>
    SyntaxClass GetMember(const LuaSyntaxTree &t) const {
        auto child = _node.FindChild(t, SyntaxClass::CanCast);
        return SyntaxClass(child);
    }

    template<class SyntaxClass>
    SyntaxClass GetMember(std::size_t skipNum, const LuaSyntaxTree &t) const {
        auto child = _node.FindChild(t, [&skipNum](auto k) -> bool {
            if (SyntaxClass::CanCast(k)) {
                if (skipNum == 0) {
                    return true;
                }
                skipNum--;
            }
            return false;
        });
        return SyntaxClass(child);
    }

    template<class SyntaxClass>
    std::vector<SyntaxClass> GetMembers(const LuaSyntaxTree &t) const {
        auto children = _node.FindChildren(t, SyntaxClass::CanCast);
        std::vector<SyntaxClass> result;
        for (auto child: children) {
            result.emplace_back(child);
        }
        return result;
    }

    template<class Visitor>
    void Accept(const Visitor &visitor, const LuaSyntaxTree &t) {
        visitor.Visit(_node, t);
    }

protected:
    LuaNodeOrToken _node;
};
