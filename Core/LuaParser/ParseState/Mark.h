#pragma once

#include <string>
#include <vector>

#include "LuaParser/Kind/LuaSyntaxNodeKind.h"
#include "LuaParser/Kind/LuaTokenKind.h"

class ParseState;

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
    CompleteMarker Complete(ParseState& p, LuaSyntaxNodeKind kind);
    void Undo(ParseState& p);
    std::size_t Pos;
};

struct CompleteMarker {
    CompleteMarker();
    CompleteMarker(std::size_t start, std::size_t finish, LuaSyntaxNodeKind kind);

    Marker Precede(ParseState& p);

    bool IsNone() const;

    std::size_t Start;
    std::size_t Finish;
    LuaSyntaxNodeKind Kind;
};