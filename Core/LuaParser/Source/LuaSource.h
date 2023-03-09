#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Lib/LineIndex/LineIndex.h"
#include "Lib/LSP/LSP.h"
#include "LuaParser/Define/LuaSyntaxError.h"

class LuaSource {
public:
    static LuaSource From(std::string text);

    explicit LuaSource();

    void UpdateFile(std::string &&fileText);

    void IncrementalUpdateFile(const lsp::Range &range, std::string &&text);

    void BulkUpdateFile(std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent);

    std::string_view GetSource() const;

    std::string_view Slice(TextRange range) const;

    const LineIndex &GetLineIndex() const;
protected:
    void BuildLineIndex();

    void InnerIncrementalUpdateFile(const lsp::Range &range, std::string &&text);

    std::string _source;
    LineIndex _lineIndex;
};