#pragma once

#include "Lib/LSP/LSP.h"
#include "Service.h"
#include <string_view>
#include "Workspace/Document/LuaDocument.h"

class VirtualFileSystem : public Service {
public:
    LANGUAGE_SERVICE(VirtualFileSystem);

    explicit VirtualFileSystem(LanguageServer *owner);

    void UpdateFile(std::string_view uri, const lsp::Range &range, std::string &&text);

    void UpdateFile(std::string_view uri, std::string &&text);

    void UpdateFile(std::string_view uri, std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent);

    void CloseFile(std::string_view uri);

    void DeleteFile(std::string_view uri);

    LuaDocument::DocumentId GetOrCreateId(std::string_view uri);
private:
    std::map<std::string, LuaDocument::DocumentId, std::less<>> _uri2Id;
    std::unordered_map<LuaDocument::DocumentId, std::string> _id2Uri;
};
