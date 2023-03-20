#pragma once

#include <vector>

#include "Lib/TextRange/TextRange.h"
#include "Lib/LSP/LSP.h"
#include "LineTypes.h"

class LineIndex {
public:
    LineIndex();

    void Parse(std::string &text, std::size_t startOffset = 0);

    LineCol GetLineCol(std::size_t offset) const;

    std::size_t GetLine(std::size_t offset) const;

    std::size_t GetCol(std::size_t offset) const;

    std::size_t GetOffset(const LineCol& lineCol) const;

    std::size_t GetTotalLine() const;

    lsp::Range ToLspRange(TextRange range) const;

    lsp::Position ToLspPosition(std::size_t offset) const;
private:
    std::vector<LineOffset> _newLines;
};
