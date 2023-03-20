#include "VirtualFileSystem.h"
#include "LanguageServer.h"


VirtualFileSystem::VirtualFileSystem(LanguageServer *owner)
    : Service(owner) {
}

void VirtualFileSystem::UpdateFile(std::string_view uri, const lsp::Range &range, std::string &&text) {
    auto id = GetOrCreateId(uri);
    _owner->GetWorkspace().UpdateDocument(id, range, std::move(text));
}

void VirtualFileSystem::UpdateFile(std::string_view uri, std::string &&text) {
    auto id = GetOrCreateId(uri);
    _owner->GetWorkspace().AddDocument(id, std::move(text));
}

void VirtualFileSystem::UpdateFile(std::string_view uri, std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent) {
    auto id = GetOrCreateId(uri);
    for (auto change: changeEvent) {
        _owner->GetWorkspace().UpdateDocument(id, change.range.value(), std::move(change.text));
    }
}

void VirtualFileSystem::CloseFile(std::string_view uri) {
}

LuaDocument::DocumentId VirtualFileSystem::GetOrCreateId(std::string_view uri) {
    auto it = _uri2Id.find(uri);
    if (it == _uri2Id.end()) {
        auto id = _owner->GetWorkspace().AllocDocumentId();
        _uri2Id.insert({std::string(uri), id});
        _id2Uri.insert({id, std::string(uri)});
        return id;
    } else {
        return it->second;
    }
}
