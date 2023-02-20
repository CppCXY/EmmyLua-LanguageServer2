#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Lib/LineIndex/LineIndex.h"
#include "LuaParser/Define/LuaSyntaxError.h"

class LuaFile {
public:
    explicit LuaFile(std::string &&fileText);

    void BuildLineIndex();

    std::string_view GetSource() const;

    std::string &GetSource();

    std::string_view Slice(std::size_t startOffset, std::size_t endOffset) const;

    const LineIndex &GetLineIndex() const;

    const std::vector<LuaSyntaxError> &GetErrors() const;

    void PushError(const LuaSyntaxError &luaSyntaxError);

protected:
    std::string _source;
    LineIndex _lineIndex;

    std::vector<LuaSyntaxError> _errors;
};