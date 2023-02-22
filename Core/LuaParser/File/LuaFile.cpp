#include "LuaFile.h"

LuaFile::LuaFile(std::string &&fileText)
    : _source(fileText) {
}

std::string_view LuaFile::GetSource() const {
    return _source;
}

std::string_view LuaFile::Slice(std::size_t startOffset, std::size_t endOffset) const {
    std::string_view source = _source;
    return source.substr(startOffset, endOffset - startOffset + 1);
}

const LineIndex &LuaFile::GetLineIndex() const {
    return _lineIndex;
}

void LuaFile::BuildLineIndex() {
    _lineIndex.Parse(_source);
}

std::string &LuaFile::GetSource() {
    return _source;
}

const std::vector<LuaSyntaxError> &LuaFile::GetSyntaxErrors() const {
    return _errors;
}

void LuaFile::PushSyntaxError(const LuaSyntaxError &luaSyntaxError) {
    _errors.emplace_back(luaSyntaxError);
}
