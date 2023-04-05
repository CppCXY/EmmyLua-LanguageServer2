#pragma once

#include "Compilation/Compilation.h"
#include "Document/LuaDocument.h"
#include <functional>

class LuaWorkspace {
public:
    LuaWorkspace();

    void LoadWorkspace(std::string_view workspace, const std::function<void(std::string_view uri, bool complete)>& callback);

    void AddDocument(LuaDocument::DocumentId id, std::string &&text);

    void RemoveDocument(LuaDocument::DocumentId id);

    void UpdateDocument(LuaDocument::DocumentId id, const lsp::Range &range, std::string &&text);

    LuaDocument* GetDocument(LuaDocument::DocumentId id);

    LuaDocument::DocumentId AllocDocumentId();
private:
    LuaDocument::DocumentId _idCounter;
    std::unordered_map<LuaDocument::DocumentId, std::unique_ptr<LuaDocument>> _documents;
    Compilation _compilation;
};