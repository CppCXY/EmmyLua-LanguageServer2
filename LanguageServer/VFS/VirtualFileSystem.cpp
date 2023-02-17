#include "VirtualFileSystem.h"

VirtualFileSystem::VirtualFileSystem() {
}

void VirtualFileSystem::UpdateFile(std::string_view uri, const lsp::Range &range, std::string &&text) {
    std::string uriString(uri);
    auto opFileId = _uriDB.Query(uriString);
    if (!opFileId.has_value()) {
        auto fileId = _fileDB.AllocFileId();
        opFileId = fileId;
        _uriDB.Input(uriString, std::move(fileId));
    }

    UpdateFile(opFileId.value(), range, std::move(text));
}

void VirtualFileSystem::UpdateFile(std::string_view uri, std::string &&text) {
    std::string uriString(uri);
    auto opFileId = _uriDB.Query(uriString);
    if (!opFileId.has_value()) {
        auto fileId = _fileDB.AllocFileId();
        opFileId = fileId;
        _uriDB.Input(uriString, std::move(fileId));
    }

    _fileDB.ApplyFileUpdate(opFileId.value(), std::move(text));
}

void VirtualFileSystem::UpdateFile(std::size_t fileId, const lsp::Range &range, std::string &&text) {
    auto opLuaFile = _fileDB.Query(fileId);
    if (!opLuaFile.has_value()) {
        _fileDB.ApplyFileUpdate(fileId, std::move(text));
        return;
    }

    auto vFile = VirtualFile(fileId);
    auto luaFile = vFile.GetLuaFile(*this);

    if (!luaFile) {
        return;
    }

    auto &sourceText = luaFile->GetSource();
    auto &lineIndex = luaFile->GetLineIndex();

    auto startOffset = lineIndex.GetOffset(LineCol(range.start.line, range.start.character));
    auto endOffset = lineIndex.GetOffset(LineCol(range.end.line, range.end.character));

    auto oldSize = sourceText.size();
    auto newSize = oldSize + (text.size() - (endOffset - startOffset));
    if (newSize > sourceText.capacity()) {
        auto suitableCapacity = newSize + 4096;
        sourceText.reserve(suitableCapacity);
    }

    // for insert
    if (startOffset == endOffset) {
        sourceText.insert(startOffset, text);
    }
    // for replace
    else {
        sourceText.replace(startOffset, endOffset - startOffset, text);
    }

    luaFile->BuildLineIndex();
}

void VirtualFileSystem::UpdateFile(std::string_view uri, std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {
    std::string stringUri(uri);
    auto opFileId = _uriDB.Query(stringUri);
    if (!opFileId.has_value()) {
        return;
    }
    auto fileId = opFileId.value();
    for(auto& change: changeEvent){
        auto opRange = change.range;
        if(opRange.has_value()){
            auto range = change.range.value();
            UpdateFile(fileId, range, std::move(change.text));
        }
    }
}

void VirtualFileSystem::ClearFile(std::string_view uri) {
    std::string stringUri(uri);
    auto opFileId = _uriDB.Query(stringUri);
    if (opFileId.has_value()) {
        auto fieldId = opFileId.value();
        _uriDB.Delete(stringUri);
        _fileDB.Delete(fieldId);
    }
}

VirtualFile VirtualFileSystem::GetVirtualFile(std::string_view uri) {
    std::string stringUri(uri);
    auto opFileId = _uriDB.Query(stringUri);
    if (opFileId.has_value()) {
        return VirtualFile(opFileId.value());
    }
    return VirtualFile(0);
}

UriDB &VirtualFileSystem::GetUriDB() {
    return _uriDB;
}

FileDB &VirtualFileSystem::GetFileDB() {
    return _fileDB;
}

VirtualFile VirtualFileSystem::GetVirtualFile(std::size_t fieldId) {
    return VirtualFile(fieldId);
}
