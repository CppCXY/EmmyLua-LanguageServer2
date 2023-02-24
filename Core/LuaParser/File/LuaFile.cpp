#include "LuaFile.h"
#include "LuaParser/Ast/LuaTreeBuilder.h"
#include "LuaParser/Lexer/LuaLexer.h"

LuaFile::LuaFile()
    : _syntaxTree(this) {
}

void LuaFile::BuildSyntaxTree() {
    _syntaxTree.Reset();

    LuaLexer luaLexer(_source);
    auto &tokens = luaLexer.Tokenize();

    LuaParser p(this, std::move(tokens));
    p.Parse();

    LuaTreeBuilder treeBuilder;
    treeBuilder.BuildTree(_syntaxTree, p);
}

void LuaFile::IncrementalUpdateFile(const lsp::Range &range, std::string &&text) {
    InnerIncrementalUpdateFile(range, std::move(text));
    BuildSyntaxTree();
}

void LuaFile::BulkUpdateFile(std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {
    for (auto &change: changeEvent) {
        auto opRange = change.range;
        if (opRange.has_value()) {
            auto range = change.range.value();
            InnerIncrementalUpdateFile(range, std::move(change.text));
        }
    }
    BuildSyntaxTree();
}

std::string_view LuaFile::GetSource() const {
    return _source;
}

std::string_view LuaFile::Slice(std::size_t startOffset, std::size_t endOffset) const {
    std::string_view source = _source;
    return source.substr(startOffset, endOffset - startOffset + 1);
}

void LuaFile::UpdateFile(std::string &&fileText) {
    _source = std::move(fileText);
    BuildLineIndex();
    BuildSyntaxTree();
}

const LineIndex &LuaFile::GetLineIndex() const {
    return _lineIndex;
}

void LuaFile::BuildLineIndex() {
    _lineIndex.Parse(_source);
}

const LuaSyntaxTree &LuaFile::GetSyntaxTree() {
    return _syntaxTree;
}

const std::vector<LuaSyntaxError> &LuaFile::GetSyntaxErrors() const {
    return _errors;
}

void LuaFile::InnerIncrementalUpdateFile(const lsp::Range &range, std::string &&text) {
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
