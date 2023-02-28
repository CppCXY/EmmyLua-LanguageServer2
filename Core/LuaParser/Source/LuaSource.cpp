#include "LuaSource.h"
#include "LuaParser/Lexer/LuaLexer.h"

LuaSource LuaSource::From(std::string text) {
    LuaSource source;
    source.UpdateFile(std::move(text));
    return source;
}

LuaSource::LuaSource() {
}

void LuaSource::IncrementalUpdateFile(const lsp::Range &range, std::string &&text) {
    InnerIncrementalUpdateFile(range, std::move(text));
}

void LuaSource::BulkUpdateFile(std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {
    for (auto &change: changeEvent) {
        auto opRange = change.range;
        if (opRange.has_value()) {
            auto range = change.range.value();
            InnerIncrementalUpdateFile(range, std::move(change.text));
        }
    }
}

std::string_view LuaSource::GetSource() const {
    return _source;
}

std::string_view LuaSource::Slice(std::size_t startOffset, std::size_t endOffset) const {
    std::string_view source = _source;
    return source.substr(startOffset, endOffset - startOffset + 1);
}

void LuaSource::UpdateFile(std::string &&fileText) {
    _source = std::move(fileText);
    BuildLineIndex();
}

const LineIndex &LuaSource::GetLineIndex() const {
    return _lineIndex;
}

void LuaSource::BuildLineIndex() {
    _lineIndex.Parse(_source);
}

const std::vector<LuaSyntaxError> &LuaSource::GetSyntaxErrors() const {
    return _errors;
}

void LuaSource::InnerIncrementalUpdateFile(const lsp::Range &range, std::string &&text) {
    auto startOffset = _lineIndex.GetOffset(LineCol(range.start.line, range.start.character));
    auto endOffset = _lineIndex.GetOffset(LineCol(range.end.line, range.end.character));

    auto oldSize = _source.size();
    auto newSize = oldSize + (text.size() - (endOffset - startOffset));
    if (newSize > _source.capacity()) {
        auto suitableCapacity = newSize + 4096;
        _source.reserve(suitableCapacity);
    }

    // for insert
    if (startOffset == endOffset) {
        _source.insert(startOffset, text);
    }
    // for replace
    else {
        _source.replace(startOffset, endOffset - startOffset, text);
    }

    BuildLineIndex();
}
