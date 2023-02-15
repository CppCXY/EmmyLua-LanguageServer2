#pragma once

#include <vector>

#include "LineTypes.h"
#include "Lib/LSP/LSP.h"
#include "LuaParser/Types/TextRange.h"

class LineIndex {
public:
    LineIndex();

    void Parse(std::string &text);

    LineCol GetLineCol(std::size_t offset);

    std::size_t GetOffset(const LineCol& lineCol);

    std::size_t GetTotalLine();

    lsp::Range ToLspRange(TextRange range);

    lsp::Position ToLspPosition(std::size_t offset);
private:
    std::vector<LineOffset> _newLines;
};
