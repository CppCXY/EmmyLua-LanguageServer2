#pragma once

#include "Lib/LineIndex/LineIndex.h"
#include <string>
#include <string_view>
#include <vector>

class LuaFile {
public:
    explicit LuaFile(std::string &&fileText);

    void BuildLineIndex();

    std::string_view GetSource() const;

    std::string &GetSource();

    std::string_view Slice(std::size_t startOffset, std::size_t endOffset) const;

    const LineIndex &GetLineIndex() const;

protected:
    std::string _source;
    LineIndex _lineIndex;
};