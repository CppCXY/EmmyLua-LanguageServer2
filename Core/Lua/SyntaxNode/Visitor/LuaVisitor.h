#include "Lua/SyntaxNode/Lua/LuaSyntaxs.h"
#include "Lua/SyntaxNode/LuaSyntaxNode.h"

class LuaVisitor {
public:
    LuaVisitor();

    template<class T>
    LuaVisitor& Visit(const std::function<void(void(T n, const LuaSyntaxTree& t))> &visitor) {
        if(T::CanCast(n)) {
            visitor(T(n), t);
        }
        std::stack<LuaNodeOrToken> traverseStack;
        traverseStack.push(syntax.GetNode());
        // 非递归深度优先遍历
        while (!traverseStack.empty()) {
            LuaNodeOrToken n = traverseStack.top();
            traverseStack.pop();

            if (n.IsNode(t)) {
                auto children = n.GetChildren(t);
                for (auto &c: children | std::views::reverse) {
                    traverseStack.push(c);
                }
            }
        }
    }
};
