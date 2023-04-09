#pragma once

#include <vector>

#include "Lua/SyntaxTree/LuaNodeOrToken.h"

template<class InputIterator, class Predicate>
class SyntaxNodeFilterIterator {
public:
    SyntaxNodeFilterIterator(InputIterator inputIt, Predicate&& predicate)
        : _inputIterator(inputIt), _predicate(predicate) {}

    [[nodiscard]] std::vector<LuaNodeOrToken> Collect(const LuaSyntaxTree &t) const {
        std::vector<LuaNodeOrToken> result;
        while (IsValid(t)) {
            result.push_back(Get());
            Next(t);
        }
        return result;
    }

    template<class SyntaxClass>
    [[nodiscard]] std::vector<SyntaxClass> Collect(const LuaSyntaxTree &t) const {
        std::vector<SyntaxClass> result;
        while (IsValid(t)) {
            result.push_back(SyntaxClass(Get()));
            Next(t);
        }
        return result;
    }

    auto Filter(LuaSyntaxNodeKind kind) {
        return SyntaxNodeFilterIterator(*this, [kind](auto n, const LuaSyntaxTree &t) {
            return n.GetSyntaxKind(t) == kind;
        });
    }

    template<class SyntaxClass>
    auto Filter() {
        return SyntaxNodeFilterIterator(*this, [](auto n, const LuaSyntaxTree &t) {
            return SyntaxClass::CanCast(n.GetSyntaxKind(t));
        });
    }

    LuaNodeOrToken Get(const LuaSyntaxTree &t) {
        auto n = _inputIterator.Get(t);
        if(_predicate(n, t)) {
            return n;
        }
        Next(t);
        return _inputIterator.Get(t);
    }

    void Next(const LuaSyntaxTree &t) {
        do {
            _inputIterator.Next();
            if (!_inputIterator.IsValid(t)) {
                break;
            }
        } while (!_predicate(Get(), t));
    }

    void IsValid(const LuaSyntaxTree &t) {
        return _inputIterator.IsValid(t);
    }

private:
    InputIterator _inputIterator;
    Predicate _predicate;
};

template <class It, class F>
SyntaxNodeFilterIterator(It it, F&& f) -> SyntaxNodeFilterIterator<It, F>;

class SyntaxNodeIterator {
public:
    explicit SyntaxNodeIterator(LuaNodeOrToken n);

    LuaNodeOrToken Get(const LuaSyntaxTree &t);

    void Next(const LuaSyntaxTree &t);

    bool IsValid(const LuaSyntaxTree &t);

private:
    LuaNodeOrToken _n;
};
