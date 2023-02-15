#pragma once

#include <string_view>
#include "Lib/LSP/LSP.h"
#include "VirtualFile.h"
#include "DB/FileDB.h"
#include "DB/UriDB.h"

class VirtualFileSystem {
public:
    VirtualFileSystem();

    void UpdateFile(std::string_view uri, const lsp::Range &range, std::string &&text);

    void UpdateFile(std::string_view uri, std::string &&text);

    void UpdateFile(std::size_t fileId, const lsp::Range &range, std::string &&text);

    void UpdateFile(std::string_view uri, std::vector<lsp::TextDocumentContentChangeEvent> &changeEvent);

    void ClearFile(std::string_view uri);

    VirtualFile GetVirtualFile(std::string_view uri);

    VirtualFile GetVirtualFile(std::size_t fieldId);

    FileDB &GetFileDB();

    UriDB &GetUriDB();

private:
    FileDB _fileDB;
    UriDB _uriDB;

    std::vector<std::string> _workspaceReadyFiles;
};
