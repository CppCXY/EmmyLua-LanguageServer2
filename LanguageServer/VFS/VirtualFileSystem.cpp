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

    luaFile->IncrementalUpdateFile(range, std::move(text));
}

void VirtualFileSystem::UpdateFile(std::string_view uri, std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {
    std::string stringUri(uri);
    auto opFileId = _uriDB.Query(stringUri);
    if (!opFileId.has_value()) {
        return;
    }
    auto fileId = opFileId.value();
    auto opFile = _fileDB.Query(fileId);
    if (opFile.has_value()) {
        opFile.value()->BulkUpdateFile(changeEvent);
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
