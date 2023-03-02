#include "LuaWorkspace.h"

LuaWorkspace::LuaWorkspace()
    : _idCounter(0) {
}

void LuaWorkspace::AddDocument(LuaDocument::DocumentId id, std::string &&text) {
    auto ptr = std::make_unique<LuaDocument>(id);
    _documents[id] = std::move(ptr);
}

void LuaWorkspace::RemoveDocument(LuaDocument::DocumentId id) {
    auto it = _documents.find(id);
    if (it != _documents.end()) {
        _documents.erase(it);
    }
}

LuaDocument *LuaWorkspace::GetDocument(LuaDocument::DocumentId id) {
    auto it = _documents.find(id);
    if (it != _documents.end()) {
        return it->second.get();
    }
    return nullptr;
}

LuaDocument::DocumentId LuaWorkspace::AllocDocumentId() {
    return _idCounter++;
}
