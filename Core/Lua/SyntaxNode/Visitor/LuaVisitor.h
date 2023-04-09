#pragma once

#include "Lua/SyntaxTree/LuaNodeOrToken.h"
#include <functional>
#include <ranges>

template<class Syntax, class F>
struct SyntaxVisitor {
    explicit SyntaxVisitor(F &&f) : _f(f) {
    }

    bool CanCast(LuaSyntaxNodeKind kind) const {
        return Syntax::CanCast(kind);
    }
    void operator()(LuaNodeOrToken n, const LuaSyntaxTree &t) const {
        _f(Syntax(n), t);
    }

private:
    F _f;
};

template<class Syntax, class F>
auto Visit(F &&f) {
    return SyntaxVisitor<Syntax, F>(std::forward<F &&>(f));
}

template<class... SyntaxVisit>
struct LuaVisitor {
    LuaVisitor(SyntaxVisit... visit)
        : _tupleVisitor{visit...} {}

    void Visit(LuaNodeOrToken node, const LuaSyntaxTree &t) const {
        if (node.IsNull(t)) {
            return;
        }

        std::apply([node, &t](auto &&...visitor) {
            bool stop = false;
            ([&]() {
                if (!stop && visitor.CanCast(node.GetSyntaxKind(t))) {
                    stop = true;
                    visitor(node, t);
                }
            }(),
             ...);
        },
                   _tupleVisitor);
    }

    void VisitChildren(LuaNodeOrToken node, const LuaSyntaxTree &t) const {
        if (node.IsNull(t)) {
            return;
        }


        std::stack<LuaNodeOrToken> traverseStack;
        for (auto &c: node.GetChildren(t) | std::views::reverse) {
            traverseStack.push(c);
        }
        // 非递归深度优先遍历
        while (!traverseStack.empty()) {
            LuaNodeOrToken n = traverseStack.top();
            traverseStack.pop();

            std::apply([n, &t](auto &&...visitor) {
                bool stop = false;
                ([&]() {
                    if (!stop && visitor.CanCast(n.GetSyntaxKind(t))) {
                        stop = true;
                        visitor(n, t);
                    }
                }(),
                 ...);
            },
                       _tupleVisitor);

            if (n.IsNode(t)) {
                auto children = n.GetChildren(t);
                for (auto &c: children | std::views::reverse) {
                    traverseStack.push(c);
                }
            }
        }
    }

    void VisitSelfAndChildren(LuaNodeOrToken node, const LuaSyntaxTree &t) const {
        if (node.IsNull(t)) {
            return;
        }


        std::stack<LuaNodeOrToken> traverseStack;
        traverseStack.push(node);
        // 非递归深度优先遍历
        while (!traverseStack.empty()) {
            LuaNodeOrToken n = traverseStack.top();
            traverseStack.pop();

            std::apply([n, &t](auto &&...visitor) {
                bool stop = false;
                ([&]() {
                    if (!stop && visitor.CanCast(n.GetSyntaxKind(t))) {
                        stop = true;
                        visitor(n, t);
                    }
                }(),
                 ...);
            },
                       _tupleVisitor);

            if (n.IsNode(t)) {
                auto children = n.GetChildren(t);
                for (auto &c: children | std::views::reverse) {
                    traverseStack.push(c);
                }
            }
        }
    }

private:
    std::tuple<SyntaxVisit...> _tupleVisitor;
};
template<class... SyntaxVisitor>
LuaVisitor(SyntaxVisitor...) -> LuaVisitor<SyntaxVisitor...>;
