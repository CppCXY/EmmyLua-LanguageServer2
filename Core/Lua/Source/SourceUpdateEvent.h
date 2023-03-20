#pragma once

#include "Lib/LSP/LSP.h"
#include "Lib/LineIndex/LineIndex.h"

class SourceUpdateEvent {
public:
    enum class Action {
        None,
        Add,
        Replace,
        Delete
    };

    static SourceUpdateEvent From(const LineIndex &lineIndex, const lsp::Range &range, std::string &&text);

    SourceUpdateEvent();

    Action UpdateAction;
    TextRange Range;
    std::string Text;
};
