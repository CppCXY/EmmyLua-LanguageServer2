#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Lib/LineIndex/LineIndex.h"
#include "LuaParser/Ast/LuaSyntaxTree.h"
#include "Lib/LSP/LSP.h"
#include "LuaParser/Define/LuaSyntaxError.h"

class LuaFile {
public:
    explicit LuaFile();

    void BuildSyntaxTree();

    void UpdateFile(std::string &&fileText);

    void IncrementalUpdateFile(const lsp::Range &range, std::string &&text);

    void BulkUpdateFile(std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent);

    std::string_view GetSource() const;

    std::string_view Slice(std::size_t startOffset, std::size_t endOffset) const;

    const LineIndex &GetLineIndex() const;

    const LuaSyntaxTree &GetSyntaxTree();

    const std::vector<LuaSyntaxError> &GetSyntaxErrors() const;
protected:
    void BuildLineIndex();

    void InnerIncrementalUpdateFile(const lsp::Range &range, std::string &&text);
    std::string _source;
    LineIndex _lineIndex;
    LuaSyntaxTree _syntaxTree;

    std::vector<LuaSyntaxError> _errors;
};