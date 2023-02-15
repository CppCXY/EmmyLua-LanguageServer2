#pragma once

#include "DBBase.h"
#include "Lib/LSP/LSP.h"
#include "LuaParser/File/LuaFile.h"
#include <string>
#include <vector>

class FileDB : public SharedDBBase<std::size_t, LuaFile> {
public:
    FileDB();

    std::size_t AllocFileId();

    void ApplyFileUpdate(std::size_t fileId, std::string &&text);

    void ApplyFileUpdate(std::vector<lsp::TextDocumentContentChangeEvent>& changeEvent);

private:
    std::size_t _fileIdCounter;
};
