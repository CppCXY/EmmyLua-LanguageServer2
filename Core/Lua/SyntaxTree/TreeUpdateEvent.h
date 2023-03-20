#pragma once

#include "Lua/Source/SourceUpdateEvent.h"
class LuaSyntaxTree;

class TreeUpdateEvent {
public:
    enum class Action {
        UpdateTree,
        OnlyUpdateWS,
        OnlyUpdateTokenOffset,
        UpdateFromOffset
    };

    static TreeUpdateEvent From(SourceUpdateEvent& sourceUpdateEvent, LuaSyntaxTree& t);

    TreeUpdateEvent();

    Action UpdateAction;

    SourceUpdateEvent SourceEvent;
private:
    static bool ContainSpace(std::string_view source);

    static bool OnlySpace(std::string_view source);

    void InitByAdd(SourceUpdateEvent &sourceUpdateEvent, LuaSyntaxTree &t);
};
