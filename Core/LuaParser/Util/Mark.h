#pragma once

#include "Core/LuaParser/Define/LuaTokenKind.h"
#include "LuaParser/Ast/LuaSyntaxNodeKind.h"
#include <string>
#include <vector>

class LuaParser;

enum class MarkEventType {
    NodeStart,
    EatToken,
    NodeEnd
};

struct MarkEvent {
    explicit MarkEvent(MarkEventType type = MarkEventType::NodeStart);

    MarkEventType Type;
    union {
        struct {
            std::size_t Parent;
            LuaSyntaxNodeKind Kind;
        } Start;
        struct {
            std::size_t Index;
            LuaTokenKind Kind;
        } Token;
    } U;
};

struct CompleteMarker;

struct Marker {
    explicit Marker(std::size_t pos);
    CompleteMarker Complete(LuaParser& p, LuaSyntaxNodeKind kind);
    void Undo(LuaParser& p);
    std::size_t Pos;
};

struct CompleteMarker {
    CompleteMarker();
    CompleteMarker(std::size_t start, std::size_t finish, LuaSyntaxNodeKind kind);

    Marker Precede(LuaParser& p);

    bool IsNone() const;

    std::size_t Start;
    std::size_t Finish;
    LuaSyntaxNodeKind Kind;
};