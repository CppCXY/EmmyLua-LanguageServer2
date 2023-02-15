#include "FileDB.h"

FileDB::FileDB()
        : SharedDBBase<std::size_t, LuaFile>(), _fileIdCounter(1) {

}

std::size_t FileDB::AllocFileId() {
    return _fileIdCounter++;
}

void FileDB::ApplyFileUpdate(std::size_t fileId, std::string &&text) {
    auto luaFile = std::make_shared<LuaFile>(std::move(text));
    luaFile->BuildLineIndex();
    Input(fileId, std::move(luaFile));
}

void FileDB::ApplyFileUpdate(std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {

}
