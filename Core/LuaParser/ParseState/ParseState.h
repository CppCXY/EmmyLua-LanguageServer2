#pragma once

#include "LuaParser/Define/LuaToken.h"
#include "Mark.h"

class ParseState {
public:
    std::vector<MarkEvent>& GetEvents();

    Marker Mark();

private:
    std::vector<MarkEvent> _events;
};
