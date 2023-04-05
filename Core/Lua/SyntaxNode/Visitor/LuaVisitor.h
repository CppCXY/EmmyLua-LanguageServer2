#pragma once

#include "Lua/SyntaxTree/LuaNodeOrToken.h"
#include <functional>
#include <ranges>

template<class Syntax, class F>
struct SyntaxVisitor : F {
    using F::operator();

    bool CanCast(LuaSyntaxNodeKind kind) const {
        return Syntax::CanCast(kind);
    }
    void operator()(LuaNodeOrToken n, const LuaSyntaxTree &t) const {
        operator()(Syntax(n), t);
    }
};

template<class Syntax, class F>
auto Visit(F) {
    return SyntaxVisitor<Syntax, F>();
}

// chatgpt 教我写代码
template<typename Func, typename Tuple, std::size_t... Is>
void for_each_tuple_break_helper(Func &&func, Tuple &&tuple, std::index_sequence<Is...>) {
    auto apply_with_index = [&]<std::size_t I>(std::integral_constant<std::size_t, I>) {
        return func(std::get<I>(tuple));
    };

    bool stop = false;
    ([&]() {
        if (!stop) {
            stop = apply_with_index(std::integral_constant<std::size_t, Is>{});
        }
    }(),
     ...);
}

template<typename Func, typename... Args>
void for_each_tuple_break(Func &&func, const std::tuple<Args...> &tuple) {
    for_each_tuple_break_helper(std::forward<Func>(func), tuple, std::make_index_sequence<sizeof...(Args)>());
}

template<class... SyntaxVisit>
struct LuaVisitor {
    LuaVisitor(SyntaxVisit... visit)
        : _tupleVisitor{visit...} {}

    void Visit(LuaNodeOrToken node, const LuaSyntaxTree &t) const {
        if (node.IsNull(t)) {
            return;
        }


        std::stack<LuaNodeOrToken> traverseStack;
        traverseStack.push(node);
        // 非递归深度优先遍历
        while (!traverseStack.empty()) {
            LuaNodeOrToken n = traverseStack.top();
            traverseStack.pop();

//            for_each_tuple_break(
//                [n, &t](auto &visitor) {
//                    if (visitor.CanCast(n.GetSyntaxKind(t))) {
//                        visitor(n, t);
//                        return true;
//                    }
//                    return false;
//                },
//                _tupleVisitor);

            std::apply([n, &t](auto&&... visitor) {
                bool stop = false;
                ([&]() {
                    if (!stop && visitor.CanCast(n.GetSyntaxKind(t))) {
                        stop = true;
                        visitor(n, t);
                    }
                }(),
                 ...);
            }, _tupleVisitor);

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
